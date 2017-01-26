#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include <ai.h>

#include "Hydro3rdParty.h"

namespace {

class Fresnel
{
public:
    virtual ~Fresnel(){}
	virtual AtRGB kr(float cos_theta)=0;

	float _eta;
};

class FresnelDielectric : public Fresnel
{
public:
	FresnelDielectric() {_eta = 0.6f;}
	FresnelDielectric(float eta) { _eta = eta; }
	virtual ~FresnelDielectric(){}

	virtual AtRGB kr(float cos_theta)
	{
		return rgb( fresnel(cos_theta, _eta) );
	}
};

enum InteriorType {
	NONE,
	ATTENUATION,
	COEFFICIENTS
};

struct ShaderData
{
	ShaderData() {}

	AtSampler* diffuseSampler;
	AtSampler* specularSampler;
	AtSampler* refractionSampler;
	int giRefractionDepth;
	int giDiffuseSamples;
	int giDiffuseSamplesSQR;
	int giSpecularSamples;
	int giSpecularSamplesSQR;
	int giRefractionSamples;
	int giRefractionSamplesSQR;
	int diffuseSampleOffset;
	int specularSampleOffset;
	int refractionSampleOffset;
	int totalSamples;
	AtCritSec cs;
	std::map<AtNode*, int> lightGroups;
	std::map<AtNode*, float> shadowDensities;
	bool standardAovs;
	bool transmitAovs;
	int numLights;

	// data for doing RR
	int rrDepth;
	int AASamples;
	float AASamplesInv;
	int totalDepth;
	int* permTable;
	int* permTableDiffuse;
	int* permTableSpecular;
	int xres;

	// AOV names
	std::vector<std::string> aovs;
	std::vector<std::string> aovs_rgba;

	int debug;
};

struct Range
{
	Range(const std::string& nm, bool rod=false)
	: name(nm), min(AI_INFINITE), max(-AI_INFINITE), total(0.0), n(0.0), reportOnDestruction(rod)
	{}

	~Range()
	{
		if (reportOnDestruction) report(std::cerr);
	}

	void addSample(double x)
	{
		min = std::min(x, min);
		max = std::max(x, max);
		total += x;
		n++;
	}

	void report(std::ostream& os)
	{
		os << "[" << name << "] " << "min: " << min << " max: " << max << " avg: " << total/n << std::endl;
	}

	std::string name;

	double min;
	double max;
	double total;
	double n;
	bool reportOnDestruction;
};

struct Histogram
{
	Histogram(const std::string& nm, double n, double x, int b, bool nrm, bool rod=false)
	: name(nm), min(n), max(x), total(0.0), bins(NULL), nbins(b), less(0.0), more(0.0), normalize(nrm), reportOnDestruction(rod)
	{
		bins = new double[nbins];
		memset(bins, 0, sizeof(double)*nbins);
	}

	~Histogram()
	{
		if (reportOnDestruction) report(std::cerr);
	}

	void addSample(double x)
	{
		if (x < min) less++;
		else if (x >= max) more++;
		else
		{
			int b = (int)((x-min)/(max-min) * nbins);
			bins[b]++;
		}

		total++;
	}

	void report(std::ostream& os)
	{
		if (normalize)
		{
			less /= total;
			more /= total;
			for (int i = 0; i < nbins; ++i)
			{
				bins[i] /= total;
			}
		}

		os << "[" << name << "] ";
		os << std::setw(10) << less << " ";
		for (int i = 0; i < nbins; ++i)
		{
			os << std::setw(10) << bins[i] << " ";
		}
		os << std::setw(10) << more << " ";
		os << std::endl;
	}

	std::string name;
	double min;
	double max;
	double total;
	double n;
	double* bins;
	int nbins;
	double less;
	double more;
	bool normalize;
	bool reportOnDestruction;
};
}
