#include <iostream>
#include <chrono>
#include<fstream>
#include<string>
#include <strstream>
#include <vector>
#include <map>

long get_now_microseconds()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	return microseconds;
}

void FisherYates_Shuffle(int* lists, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (i + 1 < len)
		{
			// 添加随机种子
			srand((unsigned int)get_now_microseconds());

			// 生成m~n的随机数
			int m = i + 1;
			int n = len - 1;
			int rand_index = rand() % (n - m + 1) + m;

			// swap
			int tmp = lists[i];
			lists[i] = lists[rand_index];
			lists[rand_index] = tmp;
		}
	}
}

bool split_deck_line(std::string line, int& num, std::string &name)
{
	int index = line.find_first_of(' ', 0);
	
	std::string str_num = line.substr(0, index);
	num = atoi(str_num.c_str());

	name = line.substr(index + 1, line.length() - index);
	
	return true;
}

bool read_deck(const char* file_name, std::vector<std::pair<int, std::string>> &deck_main, int &main_total, 
	std::vector<std::pair<int, std::string>> &deck_side, int &side_total)
{
	std::ifstream ifs;
	ifs.open(file_name, std::ios::in);
	if (ifs.is_open())
	{
		int mode = 0;
		std::string buf;
		while (std::getline(ifs, buf))
		{
			if (buf.empty() || buf == "\n")
			{
				mode = 0;
			}
			else if (buf == "Commander")
			{
				mode = 1;
			}
			else if (buf == "Deck")
			{
				mode = 2;
			}
			else if (buf == "Sideboard")
			{
				mode = 3;
			}
			else
			{
				if (mode == 1)
				{
					// 指挥官

				}
				else if (mode == 2)
				{
					// 主牌
					int num;
					std::string name;
					if (split_deck_line(buf, num, name))
					{
						//std::cout << "num: " << num << " ,name: " << name << std::endl;
						main_total += num;
						deck_main.push_back({ num, name });
					}
				}
				else if (mode == 3)
				{
					// 副牌
					int num;
					std::string name;
					if (split_deck_line(buf, num, name))
					{
						//std::cout << "num: " << num << " ,name: " << name << std::endl;
						side_total += num;
						deck_side.push_back({ num, name });
					}
				}
			}

			// std::cout << "mode: " << mode << " ,buff: [" << buf  << "]" << std::endl;
		} // end while
		ifs.close();
	}
	else
	{
		return false;
	}
	return true;
}

void init_lists(const std::vector<std::pair<int, std::string>> &deck_main, std::map<int, std::string> &deck_map, int* lists, int len)
{
	int index = 0;
	for (auto item : deck_main)
	{
		for (size_t i = 0; i < item.first; i++)
		{
			lists[index] = index;
			deck_map[index] = item.second;
			index++;
		}
	}
}

void print(std::string info, std::map<int, std::string>& deck_map, int *lists, int print_len)
{
	printf("%s: \n{\n", info.c_str());
	for (int i = 0; i < print_len; i++)
	{
		printf("    %d %s\n", i + 1, deck_map[lists[i]].c_str());
	}
	printf("}\n\n");
}

int main()
{
	int main_total = 0;
	int side_total = 0;
	std::vector<std::pair<int, std::string>> deck_main;
	std::vector<std::pair<int, std::string>> deck_side;
	if (read_deck("deck.data", deck_main, main_total, deck_side, side_total))
	{
		std::cout << "The deck: main_total: " << main_total << " ,side_total: " << side_total << std::endl << std::endl;

        int* lists = new int[main_total];
		std::map<int, std::string> deck_map;
		init_lists(deck_main, deck_map, lists, main_total);

		for (size_t i = 0; i < 10; i++)
		{
			std::cout << "-----------------------------The starting hand at firstly of the " << i << " time simulation-----------------------------" << std::endl << std::endl;
			FisherYates_Shuffle(lists, main_total);
			FisherYates_Shuffle(lists, main_total); // 由于一开始套牌中重复牌有序，所以进行两次Shuffle
			print("The order of decks after shuffling", deck_map, lists, main_total);
            print("The starting hand at firstly", deck_map, lists, 7);
		}
        delete[]lists;
	}
}
