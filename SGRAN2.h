#include <Ougens.h>
#include <vector>
		  // the base class for this instrument
typedef struct {
	float waveSampInc = 0; 
	float ampSampInc = 0; 
	float wavePhase = 0; 
	float ampPhase = 0; 
	int dur = 0; 
	float panR = 0; 
	float panL = 0; 
	int currTime = 0; 
	bool isplaying = false;
	} Grain;


class SGRAN2 : public Instrument {

public:
	SGRAN2();
	virtual ~SGRAN2();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();
	void addgrain();
	double prob(double low,double mid,double high,double tight);
	void resetgrain(Grain* grain);
	void resetgraincounter();
	int calcgrainsrequired();

private:
	bool _configured;
	int _branch;

	double _freqLow;
	double _freqMid;
	double _freqHigh;
	double _freqTight;

	double grainDurLow;
	double grainDurMid;
	double grainDurHigh;
	double grainDurTight;

	double panLow;
	double panMid;
	double panHigh;
	double panTight;

	float amp;

	std::vector<Grain*>* grains;
	int newGrainCounter;

	int grainRateSamps;
	double grainRateVarLow;
	double grainRateVarMid;
	double grainRateVarHigh;
	double grainRateVarTight;

	double* wavetable;
	int wavetableLen;
	double* grainEnv;
	int grainEnvLen;
	float grainRate;
	void doupdate();
};

