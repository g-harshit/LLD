#include<bits/stdc++.h>
using namespace std;


class IDGenerator {
private:
    int nextID;

public:
    IDGenerator() : nextID(1) {}

    int generateID() {
        return nextID++;
    }
};

class IssueIDGenerator : public IDGenerator{
    public : 
    IssueIDGenerator() : IDGenerator(){}
};
class AgentIDGenerator : public IDGenerator{
    public : 
    AgentIDGenerator() : IDGenerator(){}
};


enum class IssueStatus { OPEN, IN_PROGRESS, RESOLVED, WAITING };
enum class IssueType {Payment, Gold}; 

class Issue{
    int id;
    string transactionID;
    IssueType type;
    IssueStatus status;
    string subject;
    string description;
    string email;
    string resolution; 

    public : 

    Issue(int id,string transactionId, IssueType type, string subject, string description, string email)
        : id(id), transactionID(transactionId), type(type), subject(subject), description(description), email(email), status(IssueStatus::OPEN) {}

    string getTransactionId() const { return transactionID; }
    IssueType getIssueType() const { return type; }
    string getSubject() const { return subject; }
    string getDescription() const { return description; }
    string getEmail() const { return email; }
    IssueStatus getStatus() const { return status; }
    int getID() const{ return id; }
    string getResolution() {return resolution; }

    void setStatus(IssueStatus newStatus) { status = newStatus; }
    void setResolution(string updatedresolution){resolution = updatedresolution; }

};

class Agent {
    int id;
    string email;
    string name;
    vector<IssueType> expertise;
    Issue* issue;
    queue<Issue*> waitingIssues;
    vector<Issue*> history;

public:
    Agent(int id,string email, string name, vector<IssueType> expertise)
        : id(id), email(email), name(name), expertise(expertise) {}

    string getEmail() const { return email; }
    string getName() const { return name; }
    vector<IssueType> getExpertise() const { return expertise; }
    Issue* getIssue() {return issue; }
    vector<Issue*> getHistory() {return history; }

    void setIssue(Issue* issue){
        this->issue = issue;
    }

    void addWaitingIssue(Issue* issue){
        waitingIssues.push(issue);
    }
    
    void addIssueToHistory(Issue* issue){
      history.push_back(issue);
    }
};


class IssueAssigningStratergy{
    public:
    virtual Agent* Find(vector<Agent*> list) = 0;
};

class DefaultIssueAssigningStratergy : public IssueAssigningStratergy{
    public:
    Agent* Find(vector<Agent*> list){
        for(Agent* a : list){
            if(a->getIssue() == NULL) return a;
        }
        return NULL;
    }
};

class WaitListAssigningStratergy{
    public:
    virtual Agent* Find(vector<Agent*> list) = 0;
};

class DefaultWaitListAssigningStratergy : public WaitListAssigningStratergy{
    public:
    Agent* Find(vector<Agent*> list){
        if(list.size()>0) return list[0];
        return NULL;
    }
};

class AgentManager{
    vector<Agent*> agentList;
    IssueAssigningStratergy* stratergy;
    WaitListAssigningStratergy* waitListStratergy;
    unordered_map<Issue*,Agent*> issueAgentMap;

    void assignToWaitList(Issue* issue){
        Agent* agent = waitListStratergy->Find(agentList);
        if(agent != NULL){
            agent->addWaitingIssue(issue);
            cout<<"Issue "<<issue->getID()<<" added to waitlist of agent "<<agent->getName()<<endl;
        }
    }

    public : 

    AgentManager(IssueAssigningStratergy *stratergy){
        this->stratergy = stratergy;
        this->waitListStratergy = new DefaultWaitListAssigningStratergy();
    }

    void addAgent(Agent* newAgent){
        agentList.push_back(newAgent);
    }

    void assignIssue(Issue* issue){
        Agent* agent = stratergy->Find(agentList);
        if(agent == NULL){
            assignToWaitList(issue);
        }else{
            agent->setIssue(issue);
            issueAgentMap[issue] = agent;
            cout<<"Issue "<<issue->getID()<<" assigned to agent "<<agent->getName()<<endl;
        }
    }
    
    void updateIssue(Issue* issue,IssueStatus status,string resolution){
      issue->setStatus(status);
      issue->setResolution(resolution);
      if(status == IssueStatus::IN_PROGRESS){
        issueAgentMap[issue]->addIssueToHistory(issue);
      }
    }
    
    void resoleveIssue(Issue* issue,string resolution){
      issue->setStatus(IssueStatus::RESOLVED);
      issue->setResolution(resolution);
       // assign waiting issue when completed
    }
};

class PaymentAgentManager : public AgentManager{
    public:
    PaymentAgentManager(IssueAssigningStratergy *stratergy) : AgentManager(stratergy){}
};


class GoldAgentManager : public AgentManager{
    public:
    GoldAgentManager(IssueAssigningStratergy *stratergy) : AgentManager(stratergy){}
};

class AgentManagerFactory{
    PaymentAgentManager* paymentRelatedAgent;
    GoldAgentManager* goldRelatedAgent;

    public:
    AgentManagerFactory(){
        paymentRelatedAgent = new PaymentAgentManager(new DefaultIssueAssigningStratergy());
        goldRelatedAgent = new GoldAgentManager(new DefaultIssueAssigningStratergy());
    }

    AgentManager* getAgentManager(IssueType type){
        switch(type){
            case IssueType::Payment: 
                return paymentRelatedAgent;
            case IssueType::Gold:
                return goldRelatedAgent;
        }
    }
};

class Filter{
    string email;
    IssueType type;
    public :
    
    Filter(string email,IssueType type): email(email), type(type){}
    
    string getEmail(){return email;}
};

class IssueManager{
    AgentManagerFactory* agentFactory;
    vector<Agent*> agentList;
    vector<Issue*> issueList;
    IssueIDGenerator* issueIDGenerator;
    AgentIDGenerator* agentIDGenerator;

    Issue* getIssueByID(int id){
        for(Issue* issue : issueList){
            if(issue->getID() == id) {
              return issue;
            }
        }
        return NULL;
    }

    public:

    IssueManager(){
        agentFactory = new AgentManagerFactory();
        issueIDGenerator = new IssueIDGenerator();
        agentIDGenerator = new AgentIDGenerator();
    }

    int createIssue(string transactionId, IssueType type, string subject, string description, string email) {
        int id = issueIDGenerator->generateID();
        Issue* issue = new Issue(id,transactionId,type,subject,description,email);
        issueList.push_back(issue);
        cout<<"Issue created against transaction "<<transactionId<<endl;
        return id;
    }

    int addAgent(string agentEmail, string agentName, vector<IssueType> expertise) {
        int id = agentIDGenerator->generateID();
        Agent* agent = new Agent(id,agentEmail,agentName,expertise);
        agentList.push_back(agent);
        for(IssueType type : expertise){
            AgentManager* agentManager = agentFactory->getAgentManager(type);
            agentManager->addAgent(agent);
        }
        cout << "Agent " << agentName << " created" << endl;
        return id;
    }

    void assignIssue(int id){
        Issue* issue = getIssueByID(id);
        AgentManager* agentManager = agentFactory->getAgentManager(issue->getIssueType());
        agentManager->assignIssue(issue);
    }
    
    vector<Issue*> getIssue(Filter* filter){
      vector<Issue*> data;
      vector<Issue*> resp;
      if(filter->getEmail() != ""){
        for(Issue* issue : data){
          if(issue->getEmail() == filter->getEmail()){
            resp.push_back(issue);
          }
        }
        data = resp;
        resp = vector<Issue*>();
      }
      return data;
    }
    
    unordered_map<Agent*,vector<Issue*>> viewAgentsWorkHistory(){
      unordered_map<Agent*,vector<Issue*>> resp;
      for(Agent* a : agentList){
        resp[a] = a->getHistory();
      }
      return resp;
    }
    
};

int main(){

    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    IssueManager* im = new IssueManager();

    int i1 = im->createIssue("1234",IssueType::Payment,"Payment sub 1","Payment desc 1","abc@gmail.com");
    int i2 = im->createIssue("12345",IssueType::Gold,"Gold sub 1","Gold desc 1","abcd@gmail.com");
    int i3 = im->createIssue("123",IssueType::Payment,"Payment sub 2","Payment desc 2","abcff@gmail.com");

    im->addAgent("agent1@gmail.com","Agent 1",vector<IssueType>{IssueType::Payment,IssueType::Gold});
    im->addAgent("agent2@gmail.com","Agent 2",vector<IssueType>{IssueType::Payment});
    
    im->assignIssue(i1);
    im->assignIssue(i2);
    im->assignIssue(i3);

}
