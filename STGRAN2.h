#include <Ougens.h>
#include <vector>
		  // the base class for this instrument
typedef struct {float waveSampInc; float ampSampInc; float currTime; float ampPhase; float endTime; float panR; float panL; bool isplaying;} Grain;

class AUDIOBUFFER {
public:
    AUDIOBUFFER(int size);
    ~AUDIOBUFFER();
    double Get(float index);
    int GetHead();
    int GetSize();
    bool GetFull();
    void Append(double samp);
    void Print();

private:
    bool _full;
    int _head;
    std::vector<double>* _buffer;
};

class STGRAN2 : public Instrument {

public:
	STGRAN2();
	virtual ~STGRAN2();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();
	void addgrain();
	double prob(double low,double mid,double high,double tight);
	void resetgrain(Grain* grain);
	void resetgraincounter();
	int calcgrainsrequired();

private:
	int branch;

	AUDIOBUFFER* buffer;
	float* in;

	double transLow;
	double transMid;
	double transHigh;
	double transTight;

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

	int grainsRequired;
	double* grainEnv;
	int grainEnvLen;
	float grainRate;
	void doupdate();
};

