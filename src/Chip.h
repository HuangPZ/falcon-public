
#pragma once
#include "tools.h"
#include "connect.h"
#include "globals.h"
using namespace std;


// extern void log_print(string str);
class Chip
{
private:
    int counter_comm[2];
    // memset(counter_comm, 0, sizeof(counter_comm));
    int counter_loc;
public:
    // memset(counter_loc, 0, sizeof(counter_loc));


    int AddTimes,AddBits,CompareTimes,CompareBits,AESTimes,AESBits;
    int XORTimes;
    int TransInBits,TransInTimes,TransOutBits,TransOutTimes;
    float ClockTime,Simtime;
    float AES_throughput,AES_latency,Add_gate_latency,frequency,Adder_numbers;


	//Constructor and initializer
	Chip();

	//Functions
    void Reset();
    void ChipGenMask(const vector<myType> & x, vector<myType>& x_dot, size_t size);
    void ChipGenMask(const vector<smallType> & x, vector<smallType>& x_dot, size_t size);
    void ChipGenShare(const vector<myType>& x, const RSSVectorMyType& x_share, RSSVectorMyType& x_dot, size_t size);
    void ChipGenShare(const vector<smallType>& x, const RSSVectorSmallType& x_share, RSSVectorSmallType& x_dot, size_t size);
    void ChipPC(const vector<myType>& x, const RSSVectorMyType& x_share, const vector<myType>& c, vector<smallType>& b, 
                        size_t size);
    void ChipPC(const vector<smallType>& x, const RSSVectorSmallType& x_share, const vector<smallType>& c, vector<smallType>& b, 
                        size_t size);
    void ChipReLU(const vector<myType>& x, const RSSVectorMyType& x_share, RSSVectorMyType& y, RSSVectorSmallType& b, 
                        size_t size);
    void ChipMax(const vector<myType>& x, const RSSVectorMyType &x_share, RSSVectorMyType &max, RSSVectorSmallType &maxPrime,
 						 size_t rows, size_t columns);

};