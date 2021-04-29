
#pragma once
#include "globals.h"

class Layer
{
public: 
	int layerNum = 0;
	Layer(int _layerNum): layerNum(_layerNum) {};

//Virtual functions	
	virtual void printLayer(std::string fn) {};
	virtual void forward(const RSSVectorMyType& inputActivation) {};
	virtual void computeDelta(RSSVectorMyType& prevDelta) {};
	virtual void updateEquations(const RSSVectorMyType& prevActivations) {};

//Getters
	virtual RSSVectorMyType* getActivation() {};
	virtual RSSVectorMyType* getDelta() {};
};