/*
 * benchmark.cpp
 */
#include "benchmark.hpp"
#include "isa-support.hpp"

#include <cassert>


using namespace std;

arg_provider_t constant(void *value) {
    return [=]{ return value; };
}

const arg_provider_t null_provider = constant(nullptr);

void printBenchName(Context& c, const std::string& name) {
    c.out() << setprecision(c.getPrecision()) << fixed << setw(DESC_WIDTH) << name;
}

void printBenchName(Context& c, const Benchmark& b) {
    printBenchName(c, b->getDescription());
}

void printResultLine(Context& c, const Benchmark& b, const TimingResult& result) {
    std::ostream& os = c.out();
    printBenchName(c, b);
    printAlignedMetrics(c, result.getResults());
    os << endl;
}

void printNameHeader(Context& c) {
    c.out() << setw(DESC_WIDTH) << "Benchmark";
}

void printResultHeader(Context& c) {
    // "Benchmark", "Cycles", "Nanos"
    printNameHeader(c);
    printAlignedMetrics(c, c.getTimerInfo().getMetricNames());
    c.out() << endl;
}

BenchmarkBase::BenchmarkBase(BenchArgs args) : args{std::move(args)} {}

void BenchmarkBase::runAndPrint(Context& c) {
    if (!supports(args.features)) {
        // can't run this test on this hardware
        printBenchName(c, this);
        printOneMetric(c, std::string("Skipped because hardware doesn't support required features: ") +
                container_to_string(args.features));
        c.out() << endl;
    } else {
        runAndPrintInner(c);
    }
}

std::string BenchmarkBase::getPath() const {
    return getGroup().getId() + "/" + getId();
}



