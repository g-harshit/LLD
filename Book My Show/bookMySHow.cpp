#include<bits/stdc++.h>
using namespace std;

enum City{
    Allahabad,
    Varanasi
};

class Movie{
    int id;
    string name;

    public:
    Movie(int id,string name){
        this->id = id;
        this->name = name;
    }

    int GetID(){return id;}

    string GetName(){return name;}
};

class MovieController{
    unordered_map<City,vector<Movie*>> cityMovieMap;
    vector<Movie*> movies;

    public:

    void AddMovie(City city,Movie *movie){
        cityMovieMap[city].push_back(movie);
        movies.push_back(movie);
    }

    Movie* GetMovieByName(string name){
        for(auto i : movies){
            if(i->GetName() == name) return i;
        }
        return NULL;
    }

    vector<Movie*> GetMovieByCity(City city){
        return cityMovieMap[city];
    } 
};

class Seat{
    int id;
    int number;
    string row;
    int price;

    public:
    Seat(int id,int number,string row,int price){
        this->id = id;
        this->number = number;
        this->row = row;
        this->price = price;
    }

    int GetID(){return id;}
};

class Screen{
    int id;
    vector<Seat*> seats;
    
    public:
    Screen(int id){
        this->id = id;
    }

    void AddSeats(Seat *seat){
        seats.push_back(seat);
    }

    vector<Seat*> GetSeats(){return seats;}
};

class Show{
    int id;
    vector<int> bookedSeat;
    Movie *movie;
    Screen *screen;

    public:
    Show(int id,Movie *movie){
        this->id = id;
        this->movie = movie;
    }

    void Book(int seatID){
        bookedSeat.push_back(seatID);
    }

    void SetScreen(Screen *screen){
        this->screen = screen;
    }

    Movie* GetMovie(){return movie;}

    vector<int> GetBookedSeats(){return bookedSeat;}

};

class Theater{
    int id;
    City city;
    vector<Screen*> screens;
    vector<Show*> shows;

    public:
    Theater(int id){
        this->id = id;
    }

    void SetScreen(Screen *screen){
        screens.push_back(screen);
    }

    void SetShow(Show *show){
        shows.push_back(show);
    }

    vector<Show*> GetShows(){return shows;}    
};

class TheaterController{
    unordered_map<City,vector<Theater*>> cityTheaterMap;
    vector<Theater*> theaters;

    public:

    void AddTheater(City city,Theater *theater){
        cityTheaterMap[city].push_back(theater);
        theaters.push_back(theater);
    }

    unordered_map<Theater*,vector<Show*>> GetShow(City city, Movie *movie){
        unordered_map<Theater*,vector<Show*>> resp;

        for(auto i : cityTheaterMap[city]){
            vector<Show*> shows;

            for(auto j : i->GetShows()){
                if(j->GetMovie()->GetID() == movie->GetID()){
                    shows.push_back(j);
                }
            }

            if(shows.size() > 0){
                resp[i] = shows;
            }
        }

        return resp;
    }

};

TheaterController *theaterController = new TheaterController();
MovieController *movieController = new MovieController();

void initMovie(){
    movieController->AddMovie(Allahabad,new Movie(1, "KGF"));
    movieController->AddMovie(Allahabad,new Movie(1, "RRR"));
}

Show* CreateShow(int id,string movieName,Screen* screen){
    Show *show = new Show(1,movieController->GetMovieByName("KGF"));
    show->SetScreen(screen);
    return show;
}

Screen* CreateScreen(int id){
    Screen *screen = new Screen(id);

    screen->AddSeats(new Seat(1,1,"A",500));

    return screen;
}

void Book(City city,string movie){
    unordered_map<Theater*,vector<Show*>> theaterShowMap;
    theaterShowMap = theaterController->GetShow(city,movieController->GetMovieByName(movie));

    vector<Show*> shows;

    for(auto i : theaterShowMap){
        for(auto j : i.second){
            shows.push_back(j);
        }
    }

    if(shows.size() == 0) {
        cout<<"No SHows \n";
        return;
    }

    int seatID = 1;

    for(auto i : shows[0]->GetBookedSeats()){
        if(i == seatID) {
            cout<<"Already Booked \n";
            return;
        }
    }

    shows[0]->Book(1);
    cout<<"Booking of Seat "<<seatID<<" Done \n";

    return;
}

Theater* CreateTheater(){
    Theater *theater = new Theater(1);

    Screen *screen1 = CreateScreen(1);
    
    theater->SetScreen(screen1);
    theater->SetShow(CreateShow(1,"KGF",screen1));
    theater->SetShow(CreateShow(2,"RRR",screen1));
    
    theaterController->AddTheater(Allahabad,theater);

    return theater;
}

void initTheater(){
    theaterController->AddTheater(Allahabad,CreateTheater());
}

void init(){
    initMovie();
    initTheater();
}

int main(){

    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    init();
    Book(Allahabad,"RRR");
    //Book(Allahabad,"RRR");

    return 0;
}
