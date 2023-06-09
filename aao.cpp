// This is an independent project of an individual developer.
// Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#:
// http://www.viva64.com

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <map>
#include <list>
#include <math.h>
#include <numeric>
#include <queue>
#include <list>
#include <set>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <utility>
#include <vector>
#include <random>
#include <functional>
#include <array>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>
using namespace std;

#define all(cont) cont.begin(), cont.end()
#define unwrap(pr) pr.first, pr.second
#define pridx(q, pr) q[pr.first][pr.second]
#define logn long

mt19937 gen(42);
mt19937_64 gen64(666);

string base64_encode(const string &s)
{
	static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	size_t i = 0, ix = 0, leng = s.length();
	stringstream q;

	for (i = 0, ix = leng - leng % 3; i < ix; i += 3)
	{
		q << base64_chars[(s[i] & 0xfc) >> 2];
		q << base64_chars[((s[i] & 0x03) << 4) + ((s[i + 1] & 0xf0) >> 4)];
		q << base64_chars[((s[i + 1] & 0x0f) << 2) + ((s[i + 2] & 0xc0) >> 6)];
		q << base64_chars[s[i + 2] & 0x3f];
	}
	if (ix < leng)
	{
		q << base64_chars[(s[ix] & 0xfc) >> 2];
		q << base64_chars[((s[ix] & 0x03) << 4) + (ix + 1 < leng ? (s[ix + 1] & 0xf0) >> 4 : 0)];
		q << (ix + 1 < leng ? base64_chars[((s[ix + 1] & 0x0f) << 2)] : '=');
		q << '=';
	}
	return q.str();
}

void ReplaceAll(string &wh, string from, string to)
{
	string res;

	for (int i = 0; i < wh.size(); ++i)
	{
		if (wh.size() - i >= from.size() && wh.substr(i, from.size()) == from)
		{
			res += to;
			i += from.size() - 1;
		}
		else
		{
			res += wh[i];
		}
	}

	wh = res;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage: %s <in> <out>\n  in: input file (text)\n  out: output file (base64 json)\n", argv[0]);
		return -1;
	}

	ifstream in(argv[1]);
	ofstream out(argv[2]);

	string s;
	int v;
	in >> s >> v;
	if (s != "AceAttorneyObjection")
	{
		printf("Bad input file: wrong format!\n");
		return -1;
	}

	if (v != 1)
	{
		printf("Bad input file: wrong version!\n");
		return -1;
	}

	in >> s;
	string lang;
	for (auto &&c : s) c = tolower(c);
	if (s != "-" && s != "en")
		lang = "obj_names." + s + ".txt";

	map<string, string> names;
	{
		ifstream ni(lang);
		string a, b;
		while (ni >> a >> b)
		{
			replace(all(b), '_', ' ');
			names[a] = b;
		}
	}

	map<string, int> poses;
	{
		ifstream pi("obj_poses.txt");
		int id;
		string a, b;
		while (pi >> id >> a >> b)
		{
			poses[a + ":" + b] = id;
		}
	}

	string res = "[";
	bool bFirst = true;
	string pose, name;
	int flags;
	string text;
	while (in >> pose >> name >> flags)
	{
		getline(in, text, '\n');

		int i = 0;
		for (; i < text.size() && ' ' == text[i]; ++i);
		text = text.substr(i);

		stringstream sspose(pose);
		int pose_int;
		string charname;
		sspose >> pose_int;

		if (to_string(pose_int) != pose)
		{
			// lookup
			pose_int = poses[pose];
		}
		else
		{
			// reverse lookup
			pose = find_if(all(poses), [pose_int](const pair<string, int> &bbb)
				{
					return bbb.second == pose_int;
				}
			)->second;
		}
		charname = pose.substr(0, pose.find(':'));

		int bubbletype = flags % 4;
		int domerge = flags / 4 % 2;
		int domute = flags / 8 % 2;
		int animate = 1 - flags / 16 % 2;
		int doflip = flags / 32 % 2;

		if (name == ",")
		{
			name = names["pw"];
			domute = 1;
		}
		else if (name == "-")
			name = names[charname];
		replace(all(name), '_', ' ');

		if (!bFirst)
			res += ",";

		ReplaceAll(text, "mstop", "[#bgms]");
		ReplaceAll(text, "mcross", "[#bgm1]");
		ReplaceAll(text, "all2001", "[#bgm1]");
		ReplaceAll(text, "mtrial", "[#bgm2]");
		ReplaceAll(text, "tri2001", "[#bgm2]");
		ReplaceAll(text, "mobj", "[#bgm14]");
		ReplaceAll(text, "obj2001", "[#bgm14]");
		ReplaceAll(text, "mtruth", "[#bgm17]");
		ReplaceAll(text, "tru2001", "[#bgm17]");
		ReplaceAll(text, "mtest", "[#bgm18]");
		ReplaceAll(text, "mod2001", "[#bgm18]");
		ReplaceAll(text, "msusp", "[#bgm65]");
		ReplaceAll(text, "sus2001", "[#bgm65]");
		ReplaceAll(text, "mcorn", "[#bgm75]");
		ReplaceAll(text, "cvar2001", "[#bgm75]");
		ReplaceAll(text, "mtrick", "[#bgm79]");
		ReplaceAll(text, "mlogic", "[#bgm79]");
		ReplaceAll(text, "log2001", "[#bgm79]");
		ReplaceAll(text, "minvest", "[#bgm711]");
		ReplaceAll(text, "msearch", "[#bgm711]");
		ReplaceAll(text, "src2001", "[#bgm711]");
		ReplaceAll(text, "inv2001", "[#bgm711]");
		ReplaceAll(text, "blobby", "[#bgm7086]");
		ReplaceAll(text, "def2001", "[#bgm7086]");

		ReplaceAll(text, ",", ",[#p200]");
		ReplaceAll(text, ".", ".[#p400]");
		ReplaceAll(text, "!", "![#p300]");
		ReplaceAll(text, "?", "?[#p300]");

		res += "{\"id\":-1, \"text\":\"" + text + "\", \"poseId\":" + to_string(pose_int)
			+ ",\"bubbleType\":" + to_string(bubbletype)
			+ ",\"username\":\"" + name
			+ "\",\"mergeNext\":" + (domerge ? "true" : "false")
			+ ",\"doNotTalk\":" + (domute ? "true" : "false")
			+ ",\"goNext\":false,\"poseAnimation\":" + (animate ? "true" : "false")
			+ ",\"flipped\":" + (doflip ? "true" : "false")
			+ ",\"frameActions\":[],\"frameFades\":[],\"background\":null}";

		bFirst = false;
	}
	res += "]";

	out << base64_encode(res);
	
	return 0;
}

