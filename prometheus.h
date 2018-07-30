#include <stdio.h>
#include <stdint.h>
#include <sys/statfs.h>

#include <string>
#include <map>

enum PrometheusType {
    Counter = 0,  // can't decrease
    Gauge,
    Histogram,
    Summary,
    Untyped
};

const std::string prometheus_type_string[] = {
    "counter","gauge","histogram","summary","untyped"
};

const std::string comment_prefix = "# ";
const std::string help_prefix = "# HELP ";
const std::string type_prefix = "# TYPE ";
const std::string sum_suffix = "_sum";
const std::string count_suffix = "_count";
const std::string line_end = "\n";
const std::string line_blank = " ";

namespace {
class Metrics {
public:
    Metrics(std::string name,
            std::map<std::string, std::pair<double, int64_t> > label_values,
            PrometheusType type = Untyped,
            std::string help = "",
            std::string comment = "",
            int64_t sum = 0,
            int64_t count = 0)
                : name_(name),
                  label_values_(label_values),
                  type_(type),
                  help_(help),
                  comment_(comment),
                  sum_(sum),
                  count_(count)
    {}

    bool UpdateValue(std::string label, std::pair<double, int64_t> value, bool need_refresh_time = false);
    std::string ToString();

private:
    std::string name_;
    std::map<std::string, std::pair<double, int64_t> > label_values_;
    PrometheusType type_;
    std::string help_;
    std::string comment_;

    // for Histogram and Summary
    int64_t sum_;
    int64_t count_;
};
}

// this is not thread-safe
class MetricsManager {
public:
    bool RegisterMetric(std::string name,
                        std::map<std::string, std::pair<double, int64_t> > label_values,
                        PrometheusType type = Untyped,
                        std::string help = "",
                        std::string comment = "",
                        int64_t sum = 0,
                        int64_t count = 0);

    bool UpdateValue(std::string name, std::string label, std::pair<double, int64_t> value, bool need_refresh_time = false);
    std::string ToString();

private:
    std::map<std::string, Metrics*> metrics_;
};
