#include <iostream>
#include <fstream>	// For FD
#include <cstdlib>	// This has atoi
#include <string.h>
#include <array>
#include <linux/mman.h>
#include <vector>

using namespace std;

int *fst = new int;
int *ptr = new int;
vector<int> alph;
vector<int> subs;
array<int, 26> inv_map;
vector<int> assigned;

// Send the seed here. Loop through the digits in the seed. Assign the characters to alphabets in order. 
int 
generate_map(int seed)
{

	int current;
	int cur_num;
	int subs_cnt = 0;
	while (seed > 0)
	{
		current = seed % 10;
		cur_num = alph[current];
		seed /= 10;
		subs.push_back(cur_num);
		inv_map[cur_num - *fst] = *fst + subs_cnt;
		alph.erase(alph.begin() + current);
		subs_cnt++;
	}

	for(int i = 0; i < alph.size(); i++)
	{
		subs.push_back(alph[i]);
		inv_map[alph[i]-*fst] = *fst + subs_cnt;
		subs_cnt++;
	}

	//alph.clear();

	return 0;
}


int 
encrypt(vector<int> &plainText, vector<int> &cipherText)
{

	int cur_cipher;

	for(int i = 0; i < plainText.size(); i++)
	{
		cur_cipher = (int)plainText[i] - *fst;
		cipherText.push_back(subs[cur_cipher]);
	}

	// Write it to the new file. You have to pass *fd to new file.




	return 0;
}




int 
main(int argc, char* argv[])
{

	int seed;
	char p = 'a';
	*fst = (int)p;
	*ptr = 0;

	cout << "The integer value of a is : " << *fst << endl;

	ifstream is(argv[1]);
	ofstream os(argv[2]);

	char input_file[10000];

	char c;
	int cnt = 0;

	while(is.get(c))
	{
		input_file[cnt] = c;
		cout << c;
		cnt++;
	}

	char *output_file = (char*)calloc(cnt, sizeof(int)); 

	for (int i = 0; i < 26; ++i)
	{
		alph.push_back(*fst+i);
	}


	srand(atoi(argv[3]));
	seed = rand();
	generate_map(seed);	// This should generate the map and keep it in the subs thing. You know what to do. 


	if (atoi(argv[4]) == 1) {
		for (int i = 0; i < cnt; i++) {
			output_file[i] = subs[int(input_file[i]) - *fst];
		}
	}	else {
			for (int i = 0; i < cnt; i++) {
				output_file[i] = inv_map[int(input_file[i]) - *fst];
		}
	}

	os << output_file << endl;


	
	for (int i = 0; i < 26; i++) {
		cout << "Alphabet: " << char(*fst + i) << "\tCipher: " << (char)subs[i] << "\t" << (char)inv_map[i] << endl;
	}

	delete fst, ptr;

	return 0;
}