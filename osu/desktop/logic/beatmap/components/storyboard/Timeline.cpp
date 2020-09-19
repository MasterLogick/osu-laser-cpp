//
// Created by MasterLogick on 9/12/20.
//

#include "Timeline.h"
#include <algorithm>

namespace osu {

    void Timeline::insert(Command *c) {
        cache.push_back(c);
        timestamps.push_back(c->startTime);
        timestamps.push_back(c->endTime);
    }

    void Timeline::pack() {
        std::sort(timestamps.begin(), timestamps.end());
        int max = 0, curElem = INT_MIN, curMax = 0;
        for (int i = 0; i < timestamps.size(); ++i) {
            if (curElem == timestamps[i]) { curMax++; }
            else {
                if (curMax > max) { max = curMax; }
                curMax = 1;
                curElem = timestamps[i];
            }
        }
        std::sort(cache.begin(), cache.end(),
                  [](Command *c1, Command *c2) -> bool {
                      return c1->startTime < c2->startTime ||
                             (c1->startTime == c2->startTime &&
                              c1->endTime < c2->endTime);
                  });
        auto i = std::unique(timestamps.begin(), timestamps.end());
        timestamps.erase(i, timestamps.end());
        left = cache.begin();
        right = left;
        nextTimestamp = timestamps.begin();
    }

    void Timeline::process(int time) {
        if (lastProcessTime == INT_MIN) {
            while ((*left)->startTime < time)left++;
            while ((*right)->endTime <= time && right != cache.end())right++;
            std::for_each(left, right, [time](Command *c) {
                c->process(time);
            });
            lastProcessTime = time;
        } else {
            for (int i = lastProcessTime + 1; i <= time; ++i) {
                while ((*left)->startTime < time)left++;
                while ((*right)->endTime <= time && right != cache.end())right++;
                std::for_each(left, right, [time](Command *c) {
                    c->process(time);
                });
                lastProcessTime = time;
            }
        }
    }
}