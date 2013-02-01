#include <functional>
#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>

class Evaluation{
    public:
        Evaluation(){};
        Evaluation(double accuracy, double seconds):accuracy(accuracy), seconds(seconds){};
        double accuracy = 0.0;
        double seconds = 0.0;
};

class LinePrinter{
    public:
        template<typename T> LinePrinter& operator<<(const T& obj){
            std::stringstream ss;
            ss << obj;
            lineBuffer += ss.str();
            std::cout << ss.str();
            return *this;
        }
        void deleteLine(){
            std::cout << std::setw(lineBuffer.size()) << std::setfill('\b') << "";
            std::cout << std::setw(lineBuffer.size()) << std::setfill(' ') << "";
            std::cout << std::setw(lineBuffer.size()) << std::setfill('\b') << "";
            lineBuffer.clear();
        }
    private:
        std::string lineBuffer;
};

template<typename Certificate>
Evaluation evaluate(std::function<Certificate()> generateCertificate, std::function<double(Certificate)> evaluateCertificate){
    LinePrinter out;

    Evaluation acc1(0.0, 0.0);
    Evaluation acc2(0.0, 0.0);
    double nTries = 0.0;
    int lastPercentagePoint = 0;

    while(true){
        int nRepeats = 1;
        long microseconds = 0;
        Certificate certificate;
        for(;microseconds < 10000; nRepeats *= 2 ){
            auto time1 = std::chrono::high_resolution_clock::now();
            for(int i = 0; i<nRepeats; i++){
                certificate = generateCertificate();
            }
            auto time2 = std::chrono::high_resolution_clock::now();

            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
        }
        microseconds /= (nRepeats/2);

        nTries++;

        acc1.seconds += microseconds/1000000.0;
        acc2.seconds += microseconds/1000000.0*microseconds/1000000.0;

        double accuracy = evaluateCertificate(certificate);
        acc1.accuracy += accuracy;
        acc2.accuracy += accuracy*accuracy;

        double varAccuracy = (acc2.accuracy/nTries) - (acc1.accuracy/nTries)*(acc1.accuracy/nTries);
        double varSeconds = (acc2.seconds/nTries) - (acc1.seconds/nTries)*(acc1.seconds/nTries);

        double relErrorAccuracy = 0.0;
        if(acc1.accuracy != 0){
            relErrorAccuracy = std::sqrt(std::abs((varAccuracy/nTries)))/(acc1.accuracy/nTries);
        }
        double relErrorSeconds = std::sqrt(std::abs((varSeconds/nTries)))/(acc1.seconds/nTries);
        double maxRelErrorAccuracy = .05, maxRelErrorSeconds = .05;

        double percentageAccuracy = 1.0, percentageSeconds = 1.0;
        if( relErrorAccuracy != 0.0){
            percentageAccuracy = std::pow( maxRelErrorAccuracy/relErrorAccuracy, 2);
        }
        if( relErrorSeconds != 0.0){
            percentageSeconds = std::pow( maxRelErrorSeconds/relErrorSeconds, 2);
        }
        double percentage = std::min( percentageAccuracy, percentageSeconds );
        percentage = std::min( percentage, 1.0);
        int percentagePoint = (int)(percentage*100);

        if(nTries == 1){ out << "Starting first runs: "; }
        if(nTries <= 10){ out << nTries*10 << "% "; }
        if(nTries == 11){ out.deleteLine(); out << "Starting additional runs to increase accuracy: "; }
        if(nTries > 10 && std::abs(percentagePoint - lastPercentagePoint) > 6){ out << percentagePoint << "% "; lastPercentagePoint = percentagePoint; }

        if(nTries > 10 && percentage >= 1.0){
            acc1.accuracy /= nTries;
            acc1.seconds /= nTries;
            out.deleteLine();
            return acc1;
        }

        if(nTries > 200){
            acc1.accuracy /= nTries;
            acc1.seconds /= nTries;
            out.deleteLine();
            std::cout << "Timing results are not necessarily accurate (is the function implemented?)"<<std::endl;
            return acc1;
        }
    }
}
