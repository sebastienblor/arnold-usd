#include "utils.h"

std::string availableChannels(const Bifrost::API::Component& component, std::function<bool(const Bifrost::API::Channel&)> filter){
    Bifrost::API::RefArray channels = component.channels();
    std::stringstream ss;
    for(unsigned int i = 0; i < channels.count(); ++i)
        if(filter(channels[i]))
            ss << "    " << Bifrost::API::Channel(channels[i]).name() << std::endl;
    return ss.str();
}

std::ostream& operator<<(std::ostream& out, const AtMatrix& m){
    out << "AtMatrix[";
    for(unsigned int i = 0; i < 4; ++i){
        out << "[";
        for(unsigned int j = 0; j < 4; ++j){
            out << m[i][j];
        }
        out << "]";
    }
    out << "]";
    return out;
}
