

#include "../include/AutomaticDevices.h"
#include "../include/Time.h"

AutomaticDevices:: AutomaticDevices(String name, const int id, double wattProduction, bool status, Time current_time, Time time_duration)
                    : this.name{name}, this.id{id}, this.wattProduction{wattProduction}, this.status{status}, stop_time{current_time+time_duration}{
}