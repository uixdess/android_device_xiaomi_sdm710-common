/*
   Copyright (c) 2020, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/properties.h>
#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <stdio.h>
#include <sys/_system_properties.h>
#include <sys/system_properties.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;

void property_override(char const prop[], char const value[]) {
  prop_info *pi;

  pi = (prop_info *)__system_property_find(prop);
  if (pi)
    __system_property_update(pi, value, strlen(value));
  else
    __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
	property_override(system_prop, value);
	property_override(vendor_prop, value);
}

void load_pyxisglobal()
{
	property_override_dual("ro.product.model", "ro.product.vendor.model", "Mi 9 Lite");
	property_override("ro.build.description", "pyxis-user 10 QKQ1.190828.002 V11.0.1.0.QFCMIXM release-keys");
}

void load_pyxischina()
{
	property_override_dual("ro.product.model", "ro.product.vendor.model", "MI CC 9");
	property_override("ro.build.description", "");
}

void load_vela()
{
	property_override_dual("ro.product.model", "ro.product.vendor.model", "MI CC 9 Meitu Edition");
	property_override("ro.build.description", "pyxis-user 10 QKQ1.190828.002 V11.0.3.0.QFCCNXM release-keys");
}

void vendor_load_properties()
{
	std::string region = android::base::GetProperty("ro.boot.hwc", "");

	if (region.find("CN") != std::string::npos)
	{
		load_pyxis();
	}
	else if (region.find("GLOBAL") != std::string::npos)
	{
		load_pyxisglobalchina();
	}
	else
	{
		LOG(ERROR) << __func__ << ": unexcepted region!";
	}

    const char* path = "/proc/meminfo";
    std::ifstream infile(path);
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.find("MemTotal:") != string::npos)
        {
            if (line.substr(17, 7) > "7000000") {
                load_properties("vela");
            }
        }
    }
    infile.close();
}
