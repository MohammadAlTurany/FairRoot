/********************************************************************************
 * Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/


#include <string>                                           // for basic_string
#include <functional>                                       // for __base
#include <iosfwd>                                           // for string

#include "PixelFindHits.h"                                  // for PixelFind...
#include "runFairMQDevice.h"                                // for FairMQDev...
// PixelDetector example
#include "FairMQPixelTaskProcessorBin.h"                    // for FairMQPix...

using HitFinder = FairMQPixelTaskProcessorBin<PixelFindHits>;

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
    // clang-format off
    options.add_options()
        ("task-name",   bpo::value<std::string>()->required(),                "Name of task to run")
        ("keep-data",   bpo::value<std::string>(),                            "Name of data to keep in stream")
        ("in-channel",  bpo::value<std::string>()->default_value("data-in"),  "input channel name")
        ("out-channel", bpo::value<std::string>()->default_value("data-out"), "output channel name")
        ("par-channel", bpo::value<std::string>()->default_value("param"),    "param channel name")
        ("static-pars", bpo::value<bool>()->default_value(false),             "static parameters flag");
    // clang-format on
}

FairMQDevicePtr getDevice(const FairMQProgOptions& config)
{
    std::string taskname = config.GetValue<std::string>("task-name");

    LOG(info) << "get device with setting!";

    if (taskname == "PixelFindHits") {
        return new HitFinder();
    }

    LOG(info) << "TASK \"" << taskname << "\" UNKNOWN!!!";
    return 0;
}
