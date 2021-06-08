#pragma once
#include "Chip.h"
// #include "Functionalities.h"
using namespace std;

Chip::Chip(){
    counter_comm[0]=counter_comm[1]=counter_loc=0;

    AddTimes=AddBits=CompareTimes=CompareBits=AESTimes=AESBits=XORTimes=0;
	TransInBits=TransInTimes=TransOutBits=TransOutTimes=0;
	ClockTime=0;
	Simtime=0;
	AES_throughput=111.3e9;
	AES_latency=12.6e-6;
	Add_gate_latency=40;
	frequency=870e6;
	Adder_numbers=100;
};
void Chip::Reset(){
	counter_comm[0]=counter_comm[1]=counter_loc=0;

    AddTimes=AddBits=CompareTimes=CompareBits=AESTimes=AESBits=XORTimes=0;
	TransInBits=TransInTimes=TransOutBits=TransOutTimes=0;
	ClockTime=0;
	Simtime=0;
}

void Chip::ChipGenMask(const vector<myType>& x, vector<myType>& x_dot, size_t size){
	int cbegin=clock();
	TransInBits+=sizeof(myType)*size;
	TransInTimes+=1;
	TransOutBits+=sizeof(myType)*size;
	TransOutTimes+=1;
	myType mask;
	AESTimes+=size;
	AESBits+=sizeof(myType)*size;
	AddTimes+=size;
	AddBits+=sizeof(myType)*size;
	counter_comm[0]+=size;
	ClockTime+=clock()-cbegin;
	Simtime += sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+size/100*Add_gate_latency/frequency;
}

void Chip::ChipGenMask(const vector<smallType>& x, vector<smallType>& x_dot, size_t size){
	int cbegin=clock();
	TransInBits+=sizeof(smallType)*size;
	TransInTimes+=1;
	TransOutBits+=sizeof(smallType)*size;
	TransOutTimes+=1;
	smallType mask;
	AESTimes+=size;
	AESBits+=sizeof(smallType)*size;
	// AddTimes+=size;
	AddBits+=sizeof(smallType)*size;
	counter_comm[0]+=size;
	ClockTime+=clock()-cbegin;
	// Simtime += sizeof(smallType)*size/AES_throughput+AES_latency+100/frequency+size/100*Add_gate_latency/frequency;
}


void Chip::ChipGenShare(const vector<myType>& x, const RSSVectorMyType& x_share, RSSVectorMyType& x_dot, size_t size){
	int cbegin=clock();
	TransInBits+=(sizeof(myType)+sizeof(RSSVectorMyType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSVectorMyType))*size;
	TransOutTimes+=1;
    //RSSVectorMyType mask(size);
	AESTimes+=size*2;
	AESBits+=sizeof(myType)*size*2;
	AddTimes+=size*2;
	AddBits+=sizeof(myType)*size*2;
	counter_comm[0]+=size*2;
    counter_comm[1]+=size*2;
	ClockTime+=clock()-cbegin;
	Simtime += 2*sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+2*size/100*Add_gate_latency/frequency;
}

void Chip::ChipGenShare(const vector<smallType>& x, const RSSVectorSmallType& x_share, RSSVectorSmallType& x_dot, size_t size){
	int cbegin=clock();
	TransInBits+=(sizeof(smallType)+sizeof(RSSSmallType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSSmallType))*size;
	TransOutTimes+=1;
    // RSSSmallType mask(size);
	AESTimes+=size*2;
	AESBits+=sizeof(smallType)*size*2;
	// AddTimes+=size*2;
	AddBits+=sizeof(smallType)*size*2;
	counter_comm[0]+=size*2;
    counter_comm[1]+=size*2;
	ClockTime+=clock()-cbegin;
	// Simtime += sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+size/100*Add_gate_latency/frequency;
}

void Chip::ChipPC(const vector<myType>& x, const RSSVectorMyType& x_share, const vector<myType>& c,  vector<smallType>& b, 
                        size_t size){
	int cbegin=clock();
							
	TransInBits+=(sizeof(myType)*2+sizeof(RSSVectorMyType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSVectorMyType)+sizeof(myType))*size;
	TransOutTimes+=1;
    // RSSVectorMyType sharemask(size);
	//recover secret
	AESTimes+=size;
	AESBits+=sizeof(myType)*size;
	AddTimes+=size*3;
	AddBits+=sizeof(myType)*size*3;
	//Do ReLU
	AESTimes+=size*2;
	AESBits+=sizeof(myType)*size*2;
	AddTimes+=size;
	AddBits+=sizeof(myType)*size;
	CompareTimes+=size;
	XORTimes+=size;

	counter_comm[0]+=size;
	counter_loc+=size*2;
	ClockTime+=clock()-cbegin;
	Simtime += sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+4*size/100*Add_gate_latency/frequency;
}

void Chip::ChipPC(const vector<smallType>& x, const RSSVectorSmallType& x_share, const vector<smallType>& c, vector<smallType>& b, 
                        size_t size){

	int cbegin=clock();
	TransInBits+=(sizeof(smallType)*2+sizeof(RSSSmallType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSSmallType)+sizeof(smallType))*size;
	TransOutTimes+=1;
    // RSSSmallType sharemask(size);
	//recover secret
	AESTimes+=size;
	AESBits+=sizeof(smallType)*size;
	// AddTimes+=size*3;
	AddBits+=sizeof(smallType)*size*3;
	//Do ReLU
	AESTimes+=size*2;
	AESBits+=sizeof(smallType)*size*2;
	// AddTimes+=size;
	AddBits+=sizeof(smallType)*size;
	CompareTimes+=size;
	XORTimes+=size;

	counter_comm[0]+=size;
	counter_loc+=size*2;
	ClockTime+=clock()-cbegin;
	// Simtime += sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+4*size/100*Add_gate_latency/frequency;
}


void Chip::ChipReLU(const vector<myType>& x, const RSSVectorMyType& x_share, RSSVectorMyType& y, RSSVectorSmallType& b,
 size_t size){
	int cbegin=clock();
	TransInBits+=(sizeof(myType)+sizeof(RSSVectorMyType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSVectorMyType)+sizeof(RSSVectorSmallType))*size;
	TransOutTimes+=1;
    // RSSVectorMyType sharemask(size);
	//recover secret
	AESTimes+=size;
	AESBits+=sizeof(myType)*size;
	AddTimes+=size*3;
	AddBits+=sizeof(myType)*size*3;
	//Do ReLU
	AESTimes+=size*2;
	AESBits+=sizeof(myType)*size*2;
	AddTimes+=size;
	AddBits+=sizeof(myType)*size;
	CompareTimes+=size;
	XORTimes+=size;

	counter_comm[0]+=size;
	counter_loc+=size*2;
	ClockTime+=clock()-cbegin;
	Simtime += 3*sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+4*size/100*Add_gate_latency/frequency;
}

void Chip::ChipMax(const vector<myType>& x, const RSSVectorMyType &x_share, RSSVectorMyType &max, RSSVectorSmallType &maxPrime,
 						 size_t rows, size_t columns){
	int cbegin=clock();
	int size = rows*columns;
	TransInBits+=(sizeof(myType)+sizeof(RSSVectorMyType))*size;
	TransInTimes+=1;
	TransOutBits+=(sizeof(RSSVectorMyType)+sizeof(RSSVectorSmallType))*size;
	TransOutTimes+=1;
    // RSSVectorMyType sharemask(size);
	//recover secret
	AESTimes+=size;
	AESBits+=sizeof(myType)*size;
	AddTimes+=size*3;
	AddBits+=sizeof(myType)*size*3;
	//Do Max
	AESTimes+=size*2;
	AESBits+=sizeof(myType)*size*2;
	AddTimes+=size;
	AddBits+=sizeof(myType)*size;
	CompareTimes+=size;
	XORTimes+=size;

	counter_comm[0]+=size;
	counter_loc+=size*2;
	ClockTime+=clock()-cbegin;
	Simtime += sizeof(myType)*size/AES_throughput+AES_latency+100/frequency+size*Add_gate_latency/frequency;
}