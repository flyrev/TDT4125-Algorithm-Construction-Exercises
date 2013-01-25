#include "evaluate.h"
#include "ex1.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <map>

using namespace std;
using namespace std::placeholders;

vector<uint32_t> calculateMinimumCoins_greedy(const vector<uint32_t>& coins, uint32_t value){
	vector<uint32_t> ret(coins.size(), 0);

	// Greedy
	for (int i=coins.size()-1; i>=0; i--) {
		ret[i] = value/coins[i];
		value -= ret[i] * coins[i];
	}

	//
    
	return ret;
}

uint32_t sum_num_coins(const vector<uint32_t>& coins) {
	uint32_t num = 0;
	for (int c = 0; c<coins.size(); c++)
		num+=coins[c];
	
	return num;
}

vector<uint32_t> calculateMinimumCoins(const vector<uint32_t>& coins, uint32_t value){
	vector<uint32_t> ret(coins.size(), 0);

	map<int, vector<uint32_t> > DP;

	vector<uint32_t> tmp(coins.size(), 0);

	for (int i=1; i<value; i++) {
		vector<uint32_t> tmp(coins.size(), 0);

		int min_first=-1,min_second=-1,min_numcoins;
		for (int j=0; j<coins.size(); j++) {
			if (coins[i] == value) {
				tmp[j] = 1;
				break;
			}

		}

		DP[i] = tmp;
	}
    
	return ret;
}

int main(){
	vector<uint32_t> tmp(5, 0);
	tmp[2] = 4;
	tmp[4] = 2;
	assert(sum_num_coins(tmp) == 6);
	
	//number of coins to use
	uint32_t nCoins = 50;
	//max value of coins
	uint32_t maxCoin = 1000;
	//max possible total value
	uint32_t maxValue = 1000;

	//generator used to create random numbers, seeded with current time
	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());

	//create function that generates random coins
	uniform_int_distribution<int> coinDistribution(2, maxCoin);
	auto randomCoin = bind(coinDistribution, generator);

	//create function that generates random values
	uniform_int_distribution<int> valueDistribution(maxValue/2, maxValue);
	auto randomValue = bind(valueDistribution, generator);

	//make nCoins large vector of coins, all has value 1
	vector<uint32_t> coins(nCoins, 1);
	//fill in all except the first place with random coins
	generate(coins.begin() + 1, coins.end(), randomCoin);
	//sort the coins
	sort(coins.begin(), coins.end());

	//generate a random value
	uint32_t value = randomValue();

	//evaluate the function
	Evaluation evaluation = evaluateEx1( bind(calculateMinimumCoins, coins, value), bind( evaluateCertificateEx1, _1, coins, value ) );
	printf("Function achieves accuracy %.5f and time %.5f\n", evaluation.accuracy, evaluation.seconds);

}
