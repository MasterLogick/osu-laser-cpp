//
// Created by MasterLogick on 04/07/2020.
//

#include "Parameter.h"

namespace osu {

    Parameter::Parameter(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4
        //P,(easing),(starttime),(endtime),(parameter)
        if (s.size() < 5) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTParameter;
        ptype = (ParameterType) s[4][0];
    }

    void Parameter::process(int time) {
        //todo
    }
}