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
        bool skipLastIteration = false;
        while (*nextTimestamp <= time) {
            while ((*left)->endTime < *nextTimestamp && left != cache.end())left++;
            while ((*right)->startTime <= *nextTimestamp && right != cache.end())right++;
            std::for_each(left, right, [this](Command *c) {
                c->process(*nextTimestamp);
            });
            if (*nextTimestamp == time)skipLastIteration = true;
            nextTimestamp++;
        }
        if (*nextTimestamp > time && !skipLastIteration) {
            std::for_each(left, right, [time](Command *c) {
                c->process(time);
            });
        }
    }

    int Timeline::getStartTime() {
        return timestamps.front();
    }

    int Timeline::getEndTime() {
        return timestamps.back();
    }
}