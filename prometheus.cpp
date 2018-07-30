#include "prometheus.h"

#include <sys/time.h>

int64_t CurMicroseconds() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

std::string DoubleToString(double num) {
    char buf[64];
    ::snprintf(buf, sizeof(buf) - 1, "%lf", num);
    std::string ret = buf;
    return ret;
}

std::string Int64ToString(int64_t num) {
    char buf[64];
    ::snprintf(buf, sizeof(buf) - 1, "%ld", num);
    std::string ret = buf;
    return ret;
}

bool Metrics::UpdateValue(std::string label, std::pair<double, int64_t> value, bool need_refresh_time) {
    std::map<std::string, std::pair<double, int64_t> >::iterator ite = label_values_.find(label);
    if (ite != label_values_.end()) {
        ite->second = value;  
    } else {
        std::pair<std::map<std::string, std::pair<double, int64_t> >::iterator, bool> ret = label_values_.insert(std::make_pair(label, value));
        if (ret.second == false) {
            return false;
        }
        ite = ret.first;
    }

    if (need_refresh_time) {
        ite->second.second = CurMicroseconds();
    }
    return true;
}

std::string Metrics::ToString() {
    std::string line = "";
    
    if (!comment_.empty()) {
        line = comment_prefix + comment_ + line_end;
    }

    if (!help_.empty()) {
        line += help_prefix + name_ + line_blank + help_ + line_end;
    }

    line += type_prefix + name_ + line_blank + prometheus_type_string[type_] + line_end;

    std::map<std::string, std::pair<double, int64_t> >::iterator ite = label_values_.begin();
    std::map<std::string, std::pair<double, int64_t> >::iterator ite_end = label_values_.end();
    for (; ite != ite_end; ++ite) {
        if (ite->second.second > 0) {
            line += name_ + ite->first + line_blank + DoubleToString(ite->second.first) + line_blank + Int64ToString(ite->second.second) + line_end;
        } else {
            line += name_ + ite->first + line_blank + DoubleToString(ite->second.first) + line_end;
        }
    }

    if (type_ == Histogram || type_ == Summary) {
        if (sum_ > 0) {
            line += name_ + sum_suffix + line_blank + Int64ToString(sum_) + line_end;
        }
        if (count_ > 0) {
            line += name_ + count_suffix + line_blank + Int64ToString(count_) + line_end;
        }
    }

    return line;
}

bool MetricsManager::RegisterMetric(std::string name,
                    std::map<std::string, std::pair<double, int64_t> > label_values,
                    PrometheusType type,
                    std::string help,
                    std::string comment,
                    int64_t sum,
                    int64_t count) {
    std::map<std::string, Metrics*>::iterator ite = metrics_.find(name);
    if (ite == metrics_.end()) {
        Metrics* ptr = new Metrics(name, label_values, type, help, comment, sum, count);
        metrics_.insert(std::make_pair(name, ptr));
    }

    return true;
}

bool MetricsManager::UpdateValue(std::string name, std::string label, std::pair<double, int64_t> value, bool need_refresh_time) {
    std::map<std::string, Metrics*>::iterator ite = metrics_.find(name);
    if (ite != metrics_.end()) {
        return ite->second->UpdateValue(label, value, need_refresh_time);
    }

    return false;
}

std::string MetricsManager::ToString() {
    std::string line = "";
    std::map<std::string, Metrics*>::iterator ite = metrics_.begin();
    std::map<std::string, Metrics*>::iterator ite_end = metrics_.end();
    for (; ite != ite_end; ++ite) {
        line += ite->second->ToString();
    }

    return line;
}

