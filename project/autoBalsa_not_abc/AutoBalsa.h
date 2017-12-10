#include <bits / stdc++.h>
class Gate {
  Gate(string name) : _name(name) string _name;
};
class Input : public Gate {};
class Output : public Gate {};
class Latch : public Gate {};

class AutoBalsa {
 public:
  AutoBalsa(){};
  //   /home/yoyo/Desktop/LSV/project/autoBalsa/LGSynth91/blif/s27.blif
  void parse(string path = "LGSynth91/blif/s27.blif");
  ~AutoBalsa();

 private:
};