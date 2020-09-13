//
// Created by MasterLogick on 9/12/20.
//

#include "Timeline.h"
#include <algorithm>
#include <iostream>

namespace osu {

    void Timeline::insert(CompoundCommand *c) {
        if (tl.empty()) {
            Node front;
            front.data.push_back(c);
            front.timestamp = c->startTime;
            tl.push_back(front);
            if (c->startTime != c->endTime) {
                Node back;
                back.timestamp = c->endTime;
                back.data.push_back(c);
                tl.push_back(back);
            }
        } else {
            int startTime = c->startTime, endTime = c->endTime;
            int leftIndex = 0, rightIndex = tl.size(), middleIndex = (rightIndex + leftIndex) / 2;
            std::list<Node>::iterator left = tl.begin(), right = tl.end(), middle = tl.begin(), start, end;
            std::advance(middle, (rightIndex + leftIndex) / 2);
            while (leftIndex < rightIndex) {
                if (middle->timestamp > startTime) {
                    std::advance(right, middleIndex - rightIndex);
                    rightIndex = middleIndex;
                    std::advance(middle, (leftIndex + rightIndex) / 2 - middleIndex);
                } else {
                    std::advance(left, middleIndex + 1 - leftIndex);
                    leftIndex = middleIndex + 1;
                    std::advance(middle, (leftIndex + rightIndex) / 2 - middleIndex);
                }
                middleIndex = (leftIndex + rightIndex) / 2;
            }
            if (middle == tl.end()) {
                Node front;
                front.data.push_back(c);
                front.timestamp = c->startTime;
                tl.push_back(front);
                if (c->startTime != c->endTime) {
                    Node back;
                    back.timestamp = c->endTime;
                    back.data.push_back(c);
                    tl.push_back(back);
                }
            } else if (middle->timestamp != startTime) {
                Node &next = *(middle++);
                middle--;
                Node front;
                front.timestamp = startTime;
                for (CompoundCommand *c : middle->data) {
                    if (std::find(next.data.begin(), next.data.end(), c) != next.data.end()) {
                        front.data.push_back(c);
                        continue;
                    }
                }
                middle++;
                tl.insert(middle, front);
                middle--;
            } else {
                start = middle;
                right = tl.end();
                rightIndex = tl.size();
                std::advance(middle, (leftIndex + rightIndex) / 2 - middleIndex);
                middleIndex = middleIndex = (rightIndex + leftIndex) / 2;
                while (leftIndex + 1 != rightIndex) {
                    if (middle->timestamp > endTime) {
                        std::advance(right, middleIndex - rightIndex);
                        rightIndex = middleIndex;
                        std::advance(middle, (leftIndex + rightIndex) / 2 - middleIndex);
                    } else {
                        std::advance(left, middleIndex - leftIndex);
                        leftIndex = middleIndex;
                        std::advance(middle, (leftIndex + rightIndex) / 2 - middleIndex);
                    }
                }
                if (end == tl.end()) {
                    Node back;
                    back.timestamp = endTime;
                    back.data.push_back(c);
                    tl.push_back(back);
                    end++;
                } else if (middle->timestamp != endTime) {
                    ++middle;
                    Node &next = *(middle);
                    middle--;
                    Node back;
                    back.timestamp = endTime;
                    for (CompoundCommand *c : middle->data) {
                        if (std::find(next.data.begin(), next.data.end(), c) != next.data.end()) {
                            back.data.push_back(c);
                            continue;
                        }
                    }
                    middle++;
                    tl.insert(middle, back);
                    end = middle;
                } else {
                    end = ++middle;
                }
                std::for_each(start, end, [c](Node &n) {
                    n.data.push_back(c);
                });
            }
        }
    }

    size_t Timeline::size() {
        return tl.size();
    }

    void Timeline::print() {
        for (Node &n:tl) {
            std::cout << n.timestamp << " " << n.data.size() << std::endl;
        }
    }
}