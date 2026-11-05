#pragma once
#include "CavityController.h"

// Controlled reference: same physical pulse and probe positions at two resolutions.
// Not a microstrip or scene-source calibration.
class PropagationValidation
{
public:
    struct Result
    {
        double epsilon, spacing, timestep, expectedDelay, measuredDelay, speed, relativeError;
        std::array<std::vector<float>,2> traces;
        bool passed;
    };
    void start() { cancel(); results.clear(); failure.clear(); try { beginCase(); } catch(const std::exception& e) { failure=e.what(); } }
    void cancel() { solver.reset(); }
    bool running() const { return static_cast<bool>(solver); }
    const std::vector<Result>& completed() const { return results; }
    const std::string& error() const { return failure; }
    int stepsDone() const { return solver ? steps : 0; }
    int stepsRequired() const { return limit; }
    void advance(int budget=8)
    {
        try {
            while(solver && budget-- > 0)
            {
                solver->step(); ++steps;
                for(auto& probe : probes) probe.record(steps,solver->sampleElectric(Yee3D::Z,probe.index));
                if(steps==limit)
                {
                    finishCase(); solver.reset();
                    if(results.size()<4) beginCase();
                }
            }
        } catch(const std::exception& e) { failure=e.what(); solver.reset(); }
    }
private:
    static double peakStep(const std::vector<float>& values)
    {
        const auto peak=static_cast<std::size_t>(std::max_element(values.begin(),values.end())-values.begin());
        if(peak==0 || peak+1==values.size() || values[peak]<.1f) throw std::runtime_error("Reference pulse peak not captured");
        const double a=values[peak-1],b=values[peak],c=values[peak+1];
        return peak+.5*(a-c)/(a-2*b+c);
    }
    void beginCase()
    {
        const std::size_t refinement=results.size()%2 ? 2 : 1;
        const float epsilon=results.size()<2 ? 1.f : 4.f;
        activeGrid={{160*refinement,128*refinement,2*refinement},.0005/refinement};
        activeGrid.validate();
        activeEpsilon=epsilon;
        auto next=std::make_unique<Yee3D>(activeGrid.cells,activeGrid.spacing);
        next->setPermittivity([=](const auto&){return epsilon;});
        // Uniform across the transverse interior. The sampling window ends before
        // transverse-wall disturbances can reach the centerline probes.
        for(std::size_t z=0;z<activeGrid.cells[2];++z)
            for(std::size_t y=1;y<activeGrid.cells[1];++y)
                for(std::size_t x=1;x<activeGrid.cells[0];++x)
                {
                    const double u=(x*activeGrid.spacing-.020)/.002;
                    next->injectElectric(Yee3D::Z,{x,y,z},static_cast<float>(std::exp(-.5*u*u)));
                }
        probes[0].index={60*refinement,64*refinement,refinement};
        probes[1].index={80*refinement,64*refinement,refinement};
        for(auto& p : probes) { p.clear(); p.record(0,next->sampleElectric(Yee3D::Z,p.index)); }
        steps=0;
        limit=static_cast<int>(120*refinement*std::sqrt(epsilon));
        solver=std::move(next);
    }
    void finishCase()
    {
        Result r{};
        r.epsilon=activeEpsilon; r.spacing=activeGrid.spacing; r.timestep=solver->timestep();
        for(int p=0;p<2;++p) for(std::size_t i=0;i<probes[p].count;++i) r.traces[p].push_back(probes[p].at(i));
        r.expectedDelay=.010*std::sqrt(r.epsilon)/Yee3D::vacuumSpeed;
        r.measuredDelay=(peakStep(r.traces[1])-peakStep(r.traces[0]))*r.timestep;
        r.speed=.010/r.measuredDelay;
        r.relativeError=std::abs(r.measuredDelay/r.expectedDelay-1);
        r.passed=std::isfinite(r.speed) && r.measuredDelay>0 && r.relativeError<.02;
        results.push_back(std::move(r));
    }
    std::unique_ptr<Yee3D> solver;
    GridSettings activeGrid;
    double activeEpsilon=1;
    std::array<FieldProbe,2> probes;
    int steps=0;
    std::vector<Result> results;
    std::string failure;
    int limit=0;
};
