#include <iostream>
using namespace std;

int main(){
    unsigned long x_SCV;
    unsigned long x_drone;
    unsigned long v_SCV;
    unsigned long v_drone;
    unsigned long x_minerals;
    cin >> x_SCV;
    cin >> x_drone;
    cin >> v_SCV;
    cin >> v_drone;
    cin >> x_minerals;
    bool game = true;
    bool drone_move = true;
    bool SCV_move = true;

    while(game){
        if (drone_move)
        {
            x_drone += v_drone;
        }
        if (SCV_move)
        {
            x_SCV += v_SCV;
        }

        if (x_drone >= x_minerals && x_SCV >= x_minerals)
        {
            game = false;
            cout << "both";
        }
        else{
            if (x_drone >= x_minerals){
                game = false;
                cout << "drone";
            }
            if (x_SCV >= x_minerals){
                game = false;
                cout << "SCV";
            }
        }

        drone_move = true;
        SCV_move = true;

        if (x_drone == x_SCV + 1)
        {
            drone_move = false;
        }
        if (x_SCV == x_drone + 1)
        {
            SCV_move = false;
        }

    }

    return 88;
}