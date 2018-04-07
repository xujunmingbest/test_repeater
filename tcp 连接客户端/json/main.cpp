#include "json.h"
#include <iostream>

using namespace std;


void _tmain()
{
	//字符串
	Json::Value root;


	//根节点属性
	root["name"] = Json::Value("Tsybius");
	root["age"] = Json::Value(23);
	root["sex_is_male"] = Json::Value(true);
	Json::FastWriter fast_writer;
	std::cout << fast_writer.write(root) << std::endl;   getchar();
	//子节点
	Json::Value partner;
	//子节点属性
	partner["partner_name"] = Json::Value("Galatea");
	partner["partner_age"] = Json::Value(21);
	partner["partner_sex_is_male"] = Json::Value(false);
	//子节点挂到根节点上
	root["partner"] = Json::Value(partner);
	//数组形式
	root["achievement"].append("ach1");
	root["achievement"].append("ach2");
	root["achievement"].append("ach3");
	std::cout << fast_writer.write(root) << std::endl;   getchar();


	const char* str =
		"{\"praenomen\":\"Gaius\",\"nomen\":\"Julius\",\"cognomen\":\"Caezar\","
		"\"born\":-100,\"died\":-44}";
	Json::Reader reader;




	//从字符串中读取数据

	if (reader.parse(str, root))
	{
		string praenomen = root["praenomen"].asString();
		string nomen = root["nomen"].asString();
		string cognomen = root["cognomen"].asString();
		int born = root["born"].asInt();
		int died = root["died"].asInt();
		cout << praenomen + " " + nomen + " " + cognomen
			<< " was born in year " << born
			<< ", died in year " << died << endl;
	}

	getchar();
	return;
}