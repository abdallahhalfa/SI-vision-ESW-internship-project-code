#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <sstream>
using namespace std;
const string name[7]={"AND","OR","XOR","NAND","NOT","NOR","XNOR"};
class Node{
private:
    char name;
    bool value;
public:
    Node():name('\0'),value(0){}
    Node(char nameIn):name(nameIn){}
    Node(bool valueIn):value(valueIn){}
    Node(const Node& other) {
    this ->name=other.get_Node_name();
    this->value=other.get_Node_value();
    }
    void set_Node_value(const bool valueIn){
        this -> value = valueIn;
    }
    void set_Node_name(const char nameIn){
        this -> name = nameIn;
    }
    bool get_Node_value() const{
        return value;
    }
    char get_Node_name() const{
        return this -> name;
    }
    bool AND(Node other){
        if(this -> value == true && other.get_Node_value()==true ){
            return true;}
        else {
            return false;}
    }
    Node OR(Node other){
        Node newNode;
        if(this -> value == false  && other.get_Node_value()==false ){
            newNode.set_Node_value(false);
            return newNode;}
        else {
            newNode.set_Node_value(true);
            return newNode;}
    }
    Node XOR(Node other){
        Node newNode;
        if(this -> value != other.get_Node_value()){
            newNode.set_Node_value(true);
            return newNode;}
        else {
            newNode.set_Node_value(false);
            return newNode;}
    }
    Node& operator=(const Node& other) {
    this ->name=other.get_Node_name();
    this ->value=other.get_Node_value();
    return*this;
    }
    friend ostream& operator <<(ostream& out ,Node &other);
    ~Node(){}
};
ostream& operator <<(ostream& out ,Node &other){
    out<<other.name<<other.value;
    return out;
}

class Gate{
private:
    string gate_name;
    Node input_1;
    Node input_2;
    Node output;
public:
    Gate(){}
    Gate(const Gate& other){
        this ->gate_name=other.get_gate_name();
        this->input_1=other.get_first_Node();
        this->input_2=other.get_second_Node();
        this->output=other.get_output_Node();
    }
    void set_gate_name(const string nameIn){
        this -> gate_name=nameIn;
    }
    string get_gate_name()const{
        return gate_name;
    }
    Node get_first_Node() const{
        return &input_1;
    }
    Node get_second_Node()const{
        return &input_2;
    }
    Node get_output_Node()const{
        return &output;
    }
    void set_first_value(bool inputIn_1){
        input_1.set_Node_value(inputIn_1);
    }void set_second_value(bool inputIn_2){
        input_2.set_Node_value(inputIn_2);
    }bool get_first_value()const{
        return input_1.get_Node_value();
    }bool get_second_value()const{
        return input_2.get_Node_value();
    }bool get_output_value()const{
        return output.get_Node_value();
    }
    void getdata() const{
        cout<<input_1.get_Node_value()<<" ";
        cout<<input_2.get_Node_value()<<" ";
        cout<<output.get_Node_value()<<"\n";
    }
    void AND (){
        output.set_Node_value(input_1.AND(input_2));
    }
    void OR (){
        output=input_1.OR(input_2);
    }
    void XOR (){
        output=input_1.XOR(input_2);
    }
    void NOT (){
        output.set_Node_value(!(input_1.get_Node_value()));
    }
    void NAND (){
        Node input=input_1.AND(input_2);
        output.set_Node_value(!input.get_Node_value());
    }
    void NOR (){
        Node input=input_1.OR(input_2);
        output.set_Node_value(!input.get_Node_value());
    }
    void XNOR (){
        Node input=input_1.XOR(input_2);
        output.set_Node_value(!input.get_Node_value());
    }
    bool simulateGate() {
        if(gate_name=="AND")
            AND();
        else if(gate_name=="OR")
            OR();
        else if(gate_name=="NOR")
            NOR();
        else if(gate_name=="XOR")
            XOR();
        else if(gate_name=="NAND")
            NAND();
        else if(gate_name=="XNOR")
            XNOR();
        else if(gate_name=="NOT")
            NOT();

        return output.get_Node_value();
    }
    Node findMatch(const char& input){
        if (input==input_1.get_Node_name()) return input_1;
        else if(input==input_2.get_Node_name()) return input_2;
        return output;

        }
};

class Simulator{
private:
    std::vector<Gate*> gateVector;
    std::vector<Node*> nodeVector;
    int count1= 0;
    int count2 = 0;
    Simulator(){}
    Simulator(const Simulator& other)=delete;
public:
    static Simulator& createObject(){
        static Simulator object;
        return object;
    }
     void postGate(const Gate &g){
        gateVector.push_back(new Gate (g));
    }
    void postNode(const Node &n){
        nodeVector.push_back(new Node(n));
    }
    Node* FindNode(char ch){
        Node* newnode;
        for(auto i:nodeVector){
            if (ch==(i)->get_Node_name()) return i;
        }
        return newnode;
    }
    bool Findnodematch(char c){
        for(auto i:nodeVector){
            if (c==(i)->get_Node_name()) return true;
        }return false;
    }
    void equalmatch(){
        for(auto i:nodeVector){
            for(auto j:nodeVector)
                if (i->get_Node_name()==(j)->get_Node_name()) j->set_Node_value(i->get_Node_value());
        }
    }
    void startSimulate(){
        for(auto i:gateVector){
            if(i->get_gate_name()=="NOT"){
                equalmatch();
                i->set_first_value(nodeVector.at(count1)->get_Node_value());
                i->simulateGate();
                nodeVector.at(count1+1)->set_Node_value(i->get_output_value());
                count1+=2;
            }else{
            equalmatch();
            i->set_first_value(nodeVector.at(count1)->get_Node_value());
            i->set_second_value(nodeVector.at(count1+1)->get_Node_value());
            i->simulateGate();
            nodeVector.at(count1+2)->set_Node_value(i->get_output_value());
            equalmatch();
            count1+=3;
        }}
    }
    void print_all(){int x=0;
        for(auto i:nodeVector){

        bool y=false;
        for(int j=x-1;j>=0;j--){
            if(i->get_Node_name()==nodeVector[j]->get_Node_name()){
                y=true;
                break;
            }
        }
        if(y) {x++;continue;}
        cout<<i->get_Node_name()<<": ";
        cout<<i->get_Node_value()<<endl;
        x++;
        }
    }
~Simulator(){
for(int i = 0; i < nodeVector.size(); ++i)
   delete nodeVector[i];

for(int i = 0; i < gateVector.size(); ++i)
   delete gateVector[i];
}};



class GateGenerator{

public:
    Gate& createGate(const string& c){
        Gate* newGate=new Gate;
        newGate->set_gate_name(c);
        return *newGate;
    }
    Node& createNode(const char& c){
        Node* newNode=new Node;
        newNode->set_Node_name(c);
        return *newNode;
    }
    void parseInput(Simulator* s1,string input,bool& yy){
        bool x=false;
        for(int i=0;i<7;i++){
            if(input==name[i]){
                 x=true;
                 break;
            }
        }

        if(input=="NOT"){
            for(int i=0;i<2;i++){
            char c;
            cin>>c;
            s1->postNode(createNode(c));}
            s1->postGate(createGate(input));
        }else if(x){
            for(int i=0;i<3;i++){
            char c;
            cin>>c;
            if(s1->Findnodematch(c)){
                s1->postNode(*(s1->FindNode(c)));
            }else
            s1->postNode(createNode(c));}
            s1->postGate(createGate(input));


        }else if(input=="SIM"){
            s1->startSimulate();
        }else if(input=="SET"){
            char c;
            cin>>c;
            bool v;
            cin>>v;
            (s1->FindNode(c))->set_Node_value(v);
        }else if(input=="OUT"){
            string all;
            cin>>all;
            if(all=="ALL"){
               s1->print_all();
                yy=false;
            }else{
            cout<<s1->FindNode(all[0])->get_Node_name()<<": ";
            cout<<s1->FindNode(all[0])->get_Node_value();
            cout<<endl;
        }}
    }
};


int main(){
Simulator& s1=Simulator::createObject();
GateGenerator g1;
string input;
bool x=true;
while(x){
    cin>>input;
    g1.parseInput(&s1,input,x);
}
return 0;}
