#include <vector>
#include <cmath>
#include <cassert>

double evaluateCertificateEx1( const std::vector<uint32_t>& certificate, const std::vector<uint32_t>& coins, uint32_t value ){

	if(certificate.size() != coins.size()){
		return 0.0;
	}

	uint32_t currentValue = 0;
	uint32_t nCoins = 0;
	for(uint32_t j = 0; j < coins.size(); j++){
		currentValue += certificate[j]*coins[j];
		nCoins += certificate[j];
	}

	if(currentValue == value){
		return 1.0/nCoins;
	}
	return 0.0;
}

auto evaluateEx1 = evaluate<std::vector<uint32_t>>;
