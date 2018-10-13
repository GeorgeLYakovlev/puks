// Copyright 2017 Palo Alto What Where When Club
// Author: George Yakovlev

#include "hardware/comm_selector.h"

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/serial.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils/log.h"

CommSelector::CommSelector() {
}

CommSelector::~CommSelector() {
}

std::vector<CommPort> CommSelector::get_comms() {
  std::vector<CommPort> res;
  struct dirent **entries;
  const char *kPortSubdir = "/sys/class/tty/";
  const char *kDeviceLink = "/device";
  const char *kDriver = "/driver";
  const char *kSerial = "serial8250";
  const char *kDevices = "/dev/";
  int total = scandir(kPortSubdir, &entries, NULL, NULL);
  if (total < 0)
    return res;
  while (total-- > 0) {
    if (!strcmp(".", entries[total]->d_name) ||
      !strcmp("..", entries[total]->d_name)) {
      free(entries[total]);
      continue;
    }
    std::string full_device_path(kPortSubdir);
    full_device_path.append(entries[total]->d_name);
    full_device_path.append(kDeviceLink);
    struct stat st;
    if (lstat(full_device_path.c_str(), &st) == 0 && S_ISLNK(st.st_mode)) {
      char driver_name[256] = { 0 };
      full_device_path.append(kDriver);
      if (readlink(full_device_path.c_str(),
                   driver_name, sizeof(driver_name) - 1) > 0) {
        std::string driver(basename(driver_name));
        std::string device_name(kDevices);
        device_name.append(entries[total]->d_name);
        if (!driver.compare(kSerial)) {
          Log::printf(Log::LDEBUG, "%s -> %s - testing...\n",
                      device_name.c_str(), driver.c_str());
          int f = open(device_name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
          if (f >= 0) {
            struct serial_struct serial_info;
            if (!ioctl(f, TIOCGSERIAL, &serial_info)) {
              if (serial_info.type != PORT_UNKNOWN) {
                Log::printf(Log::LDEBUG, " added!\n");
                res.push_back(CommPort(device_name.c_str()));
              } else {
                Log::printf(Log::LDEBUG, " port unknown!\n");
              }
            }
            close(f);
          }
        } else {
          Log::printf(Log::LDEBUG, "%s -> %s\n",
                      device_name.c_str(), driver.c_str());
          res.push_back(CommPort(device_name.c_str()));
        }
      }
    }
    free(entries[total]);
  }
  free(entries);
  return res;
}
