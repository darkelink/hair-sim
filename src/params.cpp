#include "params.h"

bool Params::Exists(std::string param) {
    return params.find(param) != params.end();
}

bool Params::Add(std::string param, float value) {
    if (Exists(param)) {
        return false;
    }

    params[param] = value;
    return true;
}

bool Params::Modify(std::string param, float value) {
    if (Exists(param)) {
        params[param] = value;
        return true;
    }
    return false;
}

float Params::Get_value(std::string param) {
    return params[param];
}

std::vector<std::string> Params::Get_params() {
    std::vector<std::string> v;
    v.reserve(params.size());
    for (std::map<std::string,float>::iterator it = params.begin(); it != params.end(); ++it) {
        v.push_back(it->first);
    }
    return v;
}
