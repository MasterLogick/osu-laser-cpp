//
// Created by MasterLogick on 9/12/20.
//

#include "Timeline.h"
#include <algorithm>
#include <iostream>

namespace osu {

    void Timeline::insert(Command *c) {
        if (tl.empty()) {
            Node front;
            front.timestamp = c->startTime;
            front.data.push_back(c);
            tl.push_back(front);
            if (c->startTime != c->endTime) {
                Node back;
                back.timestamp = c->endTime;
                back.data.push_back(c);
                tl.push_back(back);
            }
        } else {
            int leftPos = getTimePoint(0, tl.size(), c->startTime);
            int rightPos = getTimePoint(leftPos, tl.size(), c->endTime);
            if (leftPos == rightPos) {
                if (leftPos < 0) {
                    leftPos = -leftPos - 1;
                    insertNode(leftPos, c->startTime);
                }
                tl[leftPos].data.push_back(c);
            } else {
                if (leftPos < 0) {
                    leftPos = -leftPos - 1;
                    insertNode(leftPos, c->startTime);
                }
                if (rightPos < 0) {
                    rightPos = -rightPos - 1;
                    insertNode(rightPos, c->endTime);
                }
                rightPos++;
                std::for_each(tl.begin() + leftPos, tl.begin() + rightPos, [c](Node &n) {
                    n.data.push_back(c);
                });
            }
        }
    }

    size_t Timeline::size() {
        return tl.size();
    }

    void Timeline::printTimestampsInfo() {
        for (Node &n:tl) {
            std::cout << n.timestamp << " " << n.data.size() << std::endl;
        }
    }

    int Timeline::getTimePoint(int left, int right, int timestamp) {
        if (tl.size() == 0) {
            return -1;
        }
        int middle;
        while (left < right) {
            middle = (left + right) / 2;
            if (tl[middle].timestamp == timestamp) {
                return middle;
            } else if (tl[middle].timestamp > timestamp) {
                right = middle;
            } else {
                left = middle + 1;
            }
        }
        return -(left + 1);
    }

    void Timeline::insertNode(int pos, int timestamp) {
        Node node;
        node.timestamp = timestamp;
        if (pos != 0 && pos != tl.size()) {
            auto leftI = tl.begin() + (pos - 1);
            auto rightI = ++std::vector<Node>::iterator(leftI);
            for (Command *c1: leftI->data) {
                for (Command *c2: rightI->data) {
                    if (c2 == c1) {
                        node.data.push_back(c1);
                        break;
                    }
                }
            }
        }
        tl.insert(tl.begin() + pos, node);
    }
}