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
	void resetgrain(Grain* grain);
	int calcgrainsrequired();

private:
	float freq;
	float amp;

	std::vector<Grain*>* grains;
	bool needNewGrain;
	int newGrainCounter;
	int grainRateSamps;
	double* wavetable;
	int wavetableLen;
	double* grainEnv;
	int grainEnvLen;
	float grainDur;
	float grainRate;
	void doupdate();
};

