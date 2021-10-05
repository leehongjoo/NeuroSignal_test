#include <iostream>
#include <fstream>
#include <sstream>
#include "NeuroSignal.h"

template <typename Stream>
void reopen2(Stream& pStream, const char * pFile,
	std::ios_base::openmode pMode = ios_base::out)
{
	pStream.close();
	pStream.clear();
	pStream.open(pFile, pMode);
}

int main()
{
	const int NData = 250;
	static PROTOCOL trainingProtocol = AlphaLow;
	string s;
	double raw1[NData];
	double raw2[NData];
	double fft_temp[MaxHz];
	double fft_temp2[MaxHz];
	double araw1[15 * NData];
	double araw2[15 * NData];
	double absBand1[nBand];
	double absBand2[nBand];
	double relBand1[nBand];
	double relBand2[nBand];
	int it = 0;
	ifstream RawIn;
	RawIn.open("raw_python.csv");
	if (RawIn.fail())
	{
		cout << "fail";
		return 0;
	}
	while (RawIn.good())
	{
		getline(RawIn, s);
		istringstream ss(s); //for split
		string stringBuffer;
		int i = 0;
		while (getline(ss, stringBuffer, ',')) //split 하나씩
		{
			raw1[i] = stod(stringBuffer);
			i++;
		}
		for (int i = 0; i < NData; i++)
		{
			araw1[it*NData + i] = raw1[i];
		}
		it++;
		double* vdata;
		double* band_abs;
		double* z_score;
		double* band_rel;
		double* band_ratio;

		vdata = FFT(raw1);
		std::cout << "fft: ";
		for (int i = 0; i < MaxHz; i++)
		{
			fft_temp[i] = vdata[i];
			std::cout << fft_temp[i] << ", ";
		}
		std::cout << std::endl;
		
		band_abs = BandAbsoluteData_FFT(fft_temp);
		std::cout << "band_abs: " << std::endl;
		for (int i = 0; i < nBand; i++)
		{
			std::cout << band_abs[i] << ", ";
		}
		std::cout << std::endl;

		band_rel = BandRelativeData_FFT(fft_temp);
		std::cout << "band_rel: " << std::endl;
		for (int i = 0; i < nBand; i++)
		{
			std::cout << band_rel[i] << ", ";
		}
		std::cout << std::endl;
		band_ratio = Ratio_FFT(fft_temp);
		std::cout << "band_ratio: " << std::endl;
		for (int i = 0; i < nRatio; i++)
		{
			std::cout << band_ratio[i] << ", ";
		}
		std::cout << std::endl;
		z_score = ZScore_FFT(fft_temp);
		std::cout << "Z_score: " << std::endl;
		for (int i = 0; i < nBand; i++)
		{
			std::cout << z_score[i] << ", ";
		}
		std::cout << std::endl;
		//delete vdata;
		//delete band_abs;
		////delete z_score;
		//delete band_rel;
		//delete band_ratio;
	}
	double* band_abs1 = BandAbsoluteData_FFT(fft_temp);
	double* band_rel1 = BandRelativeData_FFT(fft_temp);
	for (int i = 0; i < nBand; i++)
	{
		absBand1[i] = band_abs1[i];
		relBand1[i] = band_rel1[i];
	}
	RawIn.close();
	RawIn.clear();
	reopen2(RawIn, "raw_python2.csv");
	if (RawIn.fail())
	{
		cout << "fail";
		return 0;
	}
	std::cout << "/////////////////////////// raw_python2 //////////////////////////////////////" << std::endl;
	it = 0;
	while (RawIn.good())
	{
		getline(RawIn, s);
		istringstream ss(s); //for split
		string stringBuffer;
		int i = 0;
		while (getline(ss, stringBuffer, ',')) //split 하나씩
		{
			raw2[i] = stod(stringBuffer);
			i++;
		}
		for (int i = 0; i < NData; i++)
		{
			araw2[it*NData + i] = raw2[i];
		}
		it++;
		double* vdata2;
		//double* band_abs2;

		vdata2 = FFT(raw2);
		std::cout << "fft2: ";
		for (int i = 0; i < MaxHz; i++)
		{
			fft_temp2[i] = vdata2[i];
			std::cout << fft_temp2[i] << ", ";
		}
		std::cout << std::endl;
	}
	RawIn.close();
	double* band_abs2 = BandAbsoluteData_FFT(fft_temp2);
	double* band_rel2 = BandRelativeData_FFT(fft_temp2);
	for (int i = 0; i < nBand; i++)
	{
		absBand2[i] = band_abs2[i];
		relBand2[i] = band_rel2[i];
	}
	/////////////////////////////////////////////// 2ch function /////////////////////////////////////////////////////////////

	double* coh;
	coh = Coherence(14 , araw1, araw2);
	for (int i = 0; i < MaxHz; i++)
	{
		std::cout << coh[i] << ",";
	}
	double cor;
	cor = Correlation(14, araw1, araw2);
	std::cout << "Correlation : " << cor << std::endl;
	double* sym;
	sym = Symmetry(araw1, araw2);
	for (int i = 0; i < NData; i++)
	{
		std::cout << sym[i] << ", ";
	}
	double* abssym;
	abssym = AbsoluteSymmetry(absBand1, absBand2);
	std::cout << "absSym: ";
	for (int i = 0; i < nBand; i++)
	{
		std::cout << abssym[i] << ", ";
	}
	std::cout << std::endl;
	double* relsym;
	relsym = RelativeSymmetry(relBand1, relBand2);
	std::cout << "relSym: ";
	for (int i = 0; i < nBand; i++)
	{
		std::cout << relsym[i] << ", ";
	}
	std::cout << std::endl;

	PtDat proto2 = protocol2(trainingProtocol, band_abs1, band_abs2);
	std::cout << "Brain Valid : " << proto2.tf << std::endl;
	/*delete coh;
	delete sym;
	delete abssym;
	delete relsym;*/
	return 0;

}

