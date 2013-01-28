#include "evaluate.h"
#include "ex1.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <map>
#include <tuple>

using namespace std;
using namespace std::placeholders;

vector<uint32_t> calculateMinimumCoins_greedy(const vector<uint32_t>& coins, uint32_t value){
	vector<uint32_t> ret(coins.size(), 0);

	for (int i=coins.size()-1; i>=0; i--) {
		ret[i] = value/coins[i];
		value -= ret[i] * coins[i];
	}
    
	return ret;
}

uint32_t num_coins(const vector<uint32_t>& coins) {
	uint32_t num = 0;
	for (int c = 0; c<coins.size(); c++)
		num+=coins[c];
	
	return num;
}

uint32_t calculate_gcd(uint32_t m, uint32_t n) {
	if ((m % n) == 0)
		return n;
	else
		return calculate_gcd(n, m % n);
}

uint32_t gcd_vector(const vector<uint32_t>& numbers) {
	uint32_t gcd = numbers[0];
	if (gcd == 1 && numbers.size() > 1)
		gcd=numbers[1];

	for (auto n : numbers) {
		if (n != 1)
			gcd = calculate_gcd(gcd, n);
	}

	return gcd;
}

bool greedy_can_be_used(const vector<uint32_t>& coins) {
	uint32_t gcd_coins = gcd_vector(coins);
	if (coins.size() == 1 || gcd_coins >= coins[1]) {
		return true;
	}

	return false;
}

vector<uint32_t> make_coin_vector(map<uint32_t, tuple<uint32_t, uint32_t, uint32_t > >& DP, const vector<uint32_t>& coins, uint32_t value) {
	vector<uint32_t> res(coins.size(), 0);	

	auto curr_tuple = DP[value];
	
	if (get<0>(curr_tuple) == 1)
		res[get<1>(curr_tuple)]++;
	else {
		uint32_t first = get<1>(curr_tuple);
		uint32_t second = get<2>(curr_tuple);
		vector<uint32_t> tmp1 = make_coin_vector(DP, coins, first);
		vector<uint32_t> tmp2 = make_coin_vector(DP, coins, second);

		for (int i=0; i<coins.size(); i++) {
			res[i] += tmp1[i];
			res[i] += tmp2[i];
		}
	}

	return res;
}

vector<uint32_t> calculateMinimumCoins_pointers(const vector<uint32_t>& coins, uint32_t value){
	// tuple: total_number_of_coins, start_pointer, end_pointer
	map<uint32_t, tuple<uint32_t, uint32_t, uint32_t > > DP;

	for (int i=coins[0]; i<=value; i++) {
		vector<uint32_t> tmp(coins.size(), 0);

		bool perfect_coin=false;
		for (int j=0; j<coins.size(); j++) {
			if (coins[j] == i) {
				tmp[j] = 1;
				perfect_coin=true;
				DP[i] = make_tuple(1, j, 0);
				break;
			}
		}
		if (!perfect_coin) {
			uint32_t min_numcoins=value+1;
			uint32_t min_first,min_second;

			for (int m=coins[0]; m<i; m++) {
				int candidate = get<0>(DP[m])+get<0>(DP[i-m]);
				if (candidate < min_numcoins) {
					min_numcoins = candidate;
					min_first = m;
					min_second=i-m;
				}
			}

			DP[i] = make_tuple(min_numcoins, min_first, min_second);
		}
	}

	
	auto res = make_coin_vector(DP, coins, value);
	return res;
}

vector<uint32_t> calculateMinimumCoins_normal(const vector<uint32_t>& coins, uint32_t value){
	vector<uint32_t> ret(coins.size(), 0);
	map<int, vector<uint32_t> > DP;

	if (greedy_can_be_used(coins)) {
		return calculateMinimumCoins_greedy(coins, value);
	}

	for (int i=coins[0]; i<=value; i++) {
		vector<uint32_t> tmp(coins.size(), 0);

		bool perfect_coin=false;
		for (int j=0; j<coins.size(); j++) {
			if (coins[j] == i) {
				tmp[j] = 1;
				perfect_coin=true;
				DP[i] = tmp;			
			}
		}
		if (!perfect_coin) {
			uint32_t min_first,min_second,min_numcoins=value+1;

			for (int m=coins[0]; m<i; m++) {
				int candidate = num_coins(DP[m])+num_coins(DP[i-m]);
				if (candidate < min_numcoins) {
					min_numcoins = candidate;
					min_first = m;
					min_second=i-m;
				}
			}
			if (min_first < 0 || min_second < 0) {
				cerr << "ERROR: Cannot make " << i << " with current coins" << endl;
				break;
			}


			for (int coin_index=0; coin_index<coins.size(); coin_index++) {
				tmp[coin_index]  = DP[min_first][coin_index];
				tmp[coin_index] += DP[min_second][coin_index];
			}

			DP[i] = tmp;
		}
	}

	ret=DP[value];
	return ret;
}

vector<uint32_t> calculateMinimumCoins(const vector<uint32_t>& coins, uint32_t value){
	/*	cout << endl;
	cout << "With pointers" << endl;
	for (auto coin : calculateMinimumCoins_pointers(coins, value))
		cout << coin << " ";
	cout << endl;

	cout << "Without pointers" << endl;
	for (auto coin : calculateMinimumCoins_normal(coins, value))
		cout << coin << " ";
	cout << endl;
	*/
	return calculateMinimumCoins_pointers(coins, value);
}

int main(void) {
	vector<uint32_t> tmp(5, 0);
	tmp[2] = 4;
	tmp[4] = 2;
	assert(num_coins(tmp) == 6);

	vector<uint32_t> testCoins(1,1);
	calculateMinimumCoins(testCoins, 1);


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
