/*
** ShortCircuit3 is Free and Open Source Software
**
** ShortCircuit is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2004-2021 by various individuals as described by the Git transaction log
**
** All source at: https://github.com/surge-synthesizer/surge.git
**
** ShortCircuit was a commercial product from 2004-2018, with Copyright and ownership
** in that period held by Claes Johanson at Vember Audio. Claes made ShortCircuit
** open source in December 2020.
*/

#include <catch2/catch2.hpp>

#include <iostream>
#include <map>
#include <vector>

#include "configuration.h"
#include "sampler.h"

TEST_CASE("Decode Path", "[config]")
{
    std::string nameOnly, ext;
    int progid, sampleid;
    fs::path out, pathOnly;

    SECTION("Windows")
    {
        fs::path p("c:\\my\\path\\filename.WAV>100|200");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("c:\\my\\path\\filename.WAV") == 0);
        REQUIRE(ext.compare("wav") == 0);
        REQUIRE(nameOnly.compare("filename") == 0);
        REQUIRE(pathOnly.compare("c:\\my\\path") == 0);
        REQUIRE(progid == 100);
        REQUIRE(sampleid == 200);
    }
    SECTION("Sample id only")
    {
        fs::path p("c:\\my\\path\\filename.WAV|200");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("c:\\my\\path\\filename.WAV") == 0);
        REQUIRE(ext.compare("wav") == 0);
        REQUIRE(nameOnly.compare("filename") == 0);
        REQUIRE(pathOnly.compare("c:\\my\\path") == 0);
        REQUIRE(progid == -1);
        REQUIRE(sampleid == 200);
    }
    SECTION("Prog id only")
    {
        fs::path p("c:\\my\\path\\filename.WAV>100");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("c:\\my\\path\\filename.WAV") == 0);
        REQUIRE(ext.compare("wav") == 0);
        REQUIRE(nameOnly.compare("filename") == 0);
        REQUIRE(pathOnly.compare("c:\\my\\path") == 0);
        REQUIRE(progid == 100);
        REQUIRE(sampleid == -1);
    }
    SECTION("Path only with progid")
    {
        fs::path p("c:\\my\\path\\>100");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("c:\\my\\path\\") == 0);
        REQUIRE(ext.empty());
        REQUIRE(nameOnly.empty());
        REQUIRE(pathOnly.compare("c:\\my\\path") == 0);
        REQUIRE(progid == 100);
        REQUIRE(sampleid == -1);
    }
    SECTION("Path only with sampleid")
    {
        fs::path p("c:\\my\\path\\|200");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("c:\\my\\path\\") == 0);
        REQUIRE(ext.empty());
        REQUIRE(nameOnly.empty());
        REQUIRE(pathOnly.compare("c:\\my\\path") == 0);
        REQUIRE(progid == -1);
        REQUIRE(sampleid == 200);
    }
#ifndef _WIN32
    SECTION("Unix/mac") 
    {
        fs::path p("/my/path/filename.WAV>100|200");

        decode_path(p, &out, &ext, &nameOnly, &pathOnly, &progid, &sampleid);
        REQUIRE(out.compare("/my/path/filename.WAV") == 0);
        REQUIRE(ext.compare("wav") == 0);
        REQUIRE(nameOnly.compare("filename") == 0);
        REQUIRE(pathOnly.compare("/my/path") == 0);
        REQUIRE(progid == 100);
        REQUIRE(sampleid == 200);

    }        
 #endif
}