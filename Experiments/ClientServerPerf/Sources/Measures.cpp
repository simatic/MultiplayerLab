#include <algorithm>
#include <numeric>
#include "Measures.h"

Measures::Measures(size_t nb_rtts_max)
: rtts(nb_rtts_max)
{
}

void Measures::add(std::chrono::duration<double, std::milli> const& elapsed)
{
    size_t index{nbRtts++};
    rtts[index] = elapsed;
}

std::string Measures::csvHeadline()
{
    return std::string { "nbReceivedMsg,nbSentMsg,ratio nbRcv/nbSent,Average (in ms),Min,Q1,Q2,Q3,Centile #99,Max"};
}

std::string Measures::asCsv(int argNbMsg, int argNbClients)
{
    rtts.resize(nbRtts);
    std::ranges::sort(rtts);

    return std::string {
            std::to_string(rtts.size()) + ","
            + std::to_string(argNbMsg * argNbClients) + ","
            + std::to_string(static_cast<double>(nbRtts) / argNbMsg / argNbClients) + ","
            + std::to_string((std::reduce(rtts.begin(), rtts.end()) / rtts.size()).count()) + ","
            + std::to_string(rtts[0].count()) + ","
            + std::to_string(rtts[rtts.size()/4].count()) + ","
            + std::to_string(rtts[rtts.size()/2].count()) + ","
            + std::to_string(rtts[rtts.size()*3/4].count()) + ","
            + std::to_string(rtts[rtts.size()*99/100].count()) + ","
            + std::to_string(rtts[rtts.size()-1].count())
    };
}

