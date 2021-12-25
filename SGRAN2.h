#include <Ougens.h>
#include <vector>
		  // the base class for this instrument
typedef struct {float waveSampInc; float ampSampInc; float wavePhase; float ampPhase; int dur; float pan; int currTime; bool isplaying;} Grain;


class SGRAN2 : public Instrument {

public:
	SGRAN2();
	virtual ~SGRAN2();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();
	void addgrain(float waveSampInc, float ampSampInc,int dur, float pan, bool isplaying);
	double prob(double low,double mid,double high,double tight);
	void resetgrain(Grain* grain);
	int calcgrainsrequired();

private:
	double freqLow;
	double freqMid;
	double freqHigh;
	double freqTight;

	double grainDurLow;
	double grainDurMid;
	double grainDurHigh;
	double grainDurTight;

	float amp;

	std::vector<Grain*>* grains;
	bool needNewGrain;
	int newGrainCounter;
	int grainRateSamps;
	int grainsRequired;
	double* wavetable;
	int wavetableLen;
	double* grainEnv;
	int grainEnvLen;
	float grainRate;
	void doupdate();
};

