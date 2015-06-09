#include "Indicator.h"

IndicatorRef Indicator::create()
{
	IndicatorRef ref = std::shared_ptr<Indicator>(new Indicator());
	ref->setup();
	return ref;
}

Indicator::Indicator() 
{
}

Indicator::~Indicator() 
{
}

void Indicator::setup()
{
}