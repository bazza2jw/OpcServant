#ifndef _STOP_WATCH_HPP_
#define _STOP_WATCH_HPP_

// Copyright Ingo Proff 2017.
// https://github.com/CrikeeIP/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)

#include <chrono>
#include <utility>
#include <iostream>

#include <vector>
#include <chrono>


namespace stopwatch{

class Stopwatch{

public:
   typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_pt;
   enum TimeFormat{ NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS };

   Stopwatch() { }
   Stopwatch(const Stopwatch &) = default;
   virtual ~Stopwatch() {}

   virtual void start(time_pt t = std::chrono::high_resolution_clock::now()){
      start_time = t;
   }

   time_pt startTime() const { return start_time;}

   template<TimeFormat fmt = TimeFormat::MILLISECONDS>
   std::uint64_t elapsed(){
      const auto end_time = std::chrono::high_resolution_clock::now();
      return ticks<fmt>(start_time, end_time);
   }
    //
    // report time in useful units
    //
    // .. in nanoseconds
    std::uint64_t elapsed_ns() { return elapsed<Stopwatch::TimeFormat::NANOSECONDS>();}
    // .. in microseconds
    std::uint64_t elapsed_mus() {return elapsed<Stopwatch::TimeFormat::MICROSECONDS>();}
    // .. in milliseconds (default template argument, therefore not needed)
    std::uint64_t elapsed_ms() { return elapsed<Stopwatch::TimeFormat::MILLISECONDS>();}
    // .. in seconds
    std::uint64_t elapsed_s() { return elapsed<Stopwatch::TimeFormat::SECONDS>();}

    template<TimeFormat fmt = TimeFormat::MILLISECONDS>
    /*!
     * \brief hasElapsed
     * \param t
     * \return true if elapsed time >= the time
     */
    bool hasElapsed(std::uint64_t t)
    {
        return (elapsed<fmt>() >= t);
    }


    static std::string show_times( const std::vector<std::uint64_t>& times ){
        std::string result("{");
        for( const auto& t : times ){
            result += std::to_string(t) + ",";
        }
        result.back() = static_cast<char>('}');
        return result;
    }

private:
   time_pt start_time;
public:
   //
   template<TimeFormat fmt = TimeFormat::MILLISECONDS>
   static std::uint64_t ticks( const time_pt& start_time, const time_pt& end_time){
      const auto duration = end_time - start_time;
      const std::uint64_t ns_count = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

      switch(fmt){
      case TimeFormat::NANOSECONDS:
      {
         return ns_count;
      }
      case TimeFormat::MICROSECONDS:
      {
         std::uint64_t up = ((ns_count/100)%10 >= 5) ? 1 : 0;
         const auto mus_count = (ns_count /1000) + up;
         return mus_count;
      }
      case TimeFormat::MILLISECONDS:
      {
         std::uint64_t up = ((ns_count/100000)%10 >= 5) ? 1 : 0;
         const auto ms_count = (ns_count /1000000) + up;
         return ms_count;
      }
      case TimeFormat::SECONDS:
      {
         std::uint64_t up = ((ns_count/100000000)%10 >= 5) ? 1 : 0;
         const auto s_count = (ns_count /1000000000) + up;
         return s_count;
      }
      }
    }
};

class LapStopwatch : public Stopwatch
{
    std::vector<time_pt> laps;
public:
    LapStopwatch() {}
    virtual void start(time_pt t = std::chrono::high_resolution_clock::now())
    {
        Stopwatch::start(t);
        laps = {t};
    }

    template<TimeFormat fmt = TimeFormat::MILLISECONDS>
    std::uint64_t lap(){
       const auto t = std::chrono::high_resolution_clock::now();
       const auto last_r = laps.back();
       laps.push_back( t );
       return ticks<fmt>(last_r, t);
    }

    template<TimeFormat fmt_total = TimeFormat::MILLISECONDS, TimeFormat fmt_lap = fmt_total>
    std::pair<std::uint64_t, std::vector<std::uint64_t>> elapsed_laps(){
       std::vector<std::uint64_t> lap_times;
       lap_times.reserve(laps.size()-1);

       for( std::size_t idx = 0; idx <= laps.size()-2; idx++){
          const auto lap_end = laps[idx+1];
          const auto lap_start = laps[idx];
          lap_times.push_back( ticks<fmt_lap>(lap_start, lap_end) );
       }

       return { ticks<fmt_total>(startTime(), laps.back()), lap_times };
    }

};



constexpr Stopwatch::TimeFormat ns = Stopwatch::TimeFormat::NANOSECONDS;
constexpr Stopwatch::TimeFormat mus = Stopwatch::TimeFormat::MICROSECONDS;
constexpr Stopwatch::TimeFormat ms = Stopwatch::TimeFormat::MILLISECONDS;
constexpr Stopwatch::TimeFormat s = Stopwatch::TimeFormat::SECONDS;




}

#endif
