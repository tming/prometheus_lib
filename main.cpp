#include "prometheus.h"

#include <stdio.h>
#include <stdint.h>
#include <sys/statfs.h>

#include <string>
#include <iostream>

// for example, according to https://prometheus.io/docs/instrumenting/exposition_formats/
void test_prometheus_metrics() {
    MetricsManager mgr;

    std::string comment = "";
    std::string help = "The total number of HTTP requests.";
    std::string name = "http_requests_total";
    std::map<std::string, std::pair<double, int64_t> > label_values;
    label_values.insert(std::make_pair("{method=\"post\",code=\"200\"}", std::make_pair(1027,1395066363000)));
    label_values.insert(std::make_pair("{method=\"post\",code=\"400\"}", std::make_pair(3,1395066363000)));
    mgr.RegisterMetric(name, label_values, Counter, help);

    comment = "Escaping in label values:";
    name = "msdos_file_access_time_seconds";
    help.clear();
    label_values.clear();
    label_values.insert(std::make_pair("{path=\"C:\\\\DIR\\\\FILE.TXT\",error=\"Cannot find file:\\n\\\"FILE.TXT\\\"\"}", std::make_pair(1.458255915e9,0)));
    mgr.RegisterMetric(name, label_values, Gauge, help, comment);
    
    comment = "Minimalistic line:";
    name = "metric_without_timestamp_and_labels";
    help.clear();
    label_values.clear();
    label_values.insert(std::make_pair("", std::make_pair(12.47,0)));
    mgr.RegisterMetric(name, label_values, Gauge, help, comment);
    
    comment = "A weird metric from before the epoch:";
    name = "something_weird";
    help.clear();
    label_values.clear();
    label_values.insert(std::make_pair("{problem=\"division by zero\"}", std::make_pair(0,-3982045)));
    mgr.RegisterMetric(name, label_values, Gauge, help, comment);
    
    comment = "A histogram, which has a pretty complex representation in the text format:";
    name = "http_request_duration_seconds";
    help = "A histogram of the request duration.";
    label_values.clear();
    label_values.insert(std::make_pair("{le=\"0.05\"}", std::make_pair(24054,0)));
    label_values.insert(std::make_pair("{le=\"0.1\"}", std::make_pair(33444,0)));
    label_values.insert(std::make_pair("{le=\"0.2\"}", std::make_pair(100392,0)));
    label_values.insert(std::make_pair("{le=\"0.5\"}", std::make_pair(129389,0)));
    label_values.insert(std::make_pair("{le=\"1\"}", std::make_pair(133988,0)));
    label_values.insert(std::make_pair("{le=\"+Inf\"}", std::make_pair(144320,0)));
    mgr.RegisterMetric(name, label_values, Histogram, help, comment, 53423, 144320);
    
    comment = "Finally a summary, which has a complex representation, too:";
    name = "rpc_duration_seconds";
    help = "A summary of the RPC duration in seconds.";
    label_values.clear();
    label_values.insert(std::make_pair("{quantile=\"0.01\"}", std::make_pair(3102,0)));
    label_values.insert(std::make_pair("{quantile=\"0.05\"}", std::make_pair(3272,0)));
    label_values.insert(std::make_pair("{quantile=\"0.5\"}", std::make_pair(4773,0)));
    label_values.insert(std::make_pair("{quantile=\"0.9\"}", std::make_pair(9001,0)));
    label_values.insert(std::make_pair("{quantile=\"0.99\"}", std::make_pair(76656,0)));
    mgr.RegisterMetric(name, label_values, Summary, help, comment, 1.7560473e+07, 2693);
    
    std::cout << mgr.ToString() << std::endl;
}

int32_t main(int argc, char** argv) {
    test_prometheus_metrics();
    
    int32_t pause;
    std::cin >> pause;
    return 0;
}
