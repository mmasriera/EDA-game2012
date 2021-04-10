#include "Player.hh"
#include <cmath>

using namespace std;

#define PLAYER_NAME _

struct PLAYER_NAME : public Player {
    static Player* factory () {
        return new PLAYER_NAME;
    }
        vector<vector<int> > mat;
        vector<Dir> desp;


virtual void play () {

        mat = vector<vector<int> >(rows(), vector<int>(cols(), -1));

        if (round() == 0) {
            desp =  vector<Dir>(4);
            desp[0] = Left; desp[1] = Top; desp[2] = Right; desp[3] = Bottom;

        }
        if( pacman(me()).alive) {
            Dir d1;

            Pos p = pacman(me()).pos;
            if (pacman(me()).type == PacMan ) d1 = bfs_pac(p);
            else d1 = bfs_pacpill(p);

            move_my_pacman( d1 );
        }
        for (int i=0; i < nb_ghosts(); ++i) {
            if( ghost(me(), i).alive ) {
                    Pos q = ghost(me(), i).pos;
                    Dir d2 = bfs_fant(q);
                    move_my_ghost(i, d2);
            }       
        }
}


inline bool moveghost (Pos p, Dir d) {
        CType T = cell(dest(p, d)).type;
        if (T == Wall) return false;
        int r = cell(dest(p, d)).id;
        if (r == -1) return true;
        if (robot(r).type==Ghost) return false;
        return true;
 }
    
inline bool movepac(Pos p, Dir d) {
        CType T = cell(dest(p, d)).type;
        if (T == Wall or T == Gate) return false;
        if (pacman(me()).type == PacMan and cell(dest(p, d)).id != -1) return false;
        return true;
}

inline Dir bfs_pac( Pos p) {

                queue<Pos> pacpos;
                bool found = false;

                mat[p.i][p.j] = 0;

                pacpos.push( p );

                while((not pacpos.empty()) and (not found)) {

                        Pos p1 = pacpos.front();
                        pacpos.pop();

                if( (cell(p1).type == Pill or cell(p1).type == Bonus or cell(p1).type == Hammer or cell(p1).type == Mushroom ) and (cell(p1).id == -1) ) {
                                found = true;

                                Pos p2;
                                while(mat[p1.i][p1.j] != 0 ) {

                                        for(int z = 0; z < 4; ++z) {
                                                p2 = dest(p1, desp[z]);
                                                if( mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1) p1 = p2;

                                                if( mat[p1.i][p1.j] == 0) {
                                                        if (desp[z] == Right) return Left ;
                                                        if (desp[z] == Left) return Right ;
                                                        if (desp[z] == Top) return Bottom ;
                                                        if (desp[z] == Bottom ) return Top ;
                                                }
                                        }
                                }

                        }
                        else {
                                for(int k = 0; k < 4; ++k) {
                                        if( movepac(p1,desp[k])) {
                                                Pos seg = dest(p1, desp[k]); //posicio seguent

                                                if( pos_ok(seg) and (mat[seg.i][seg.j] == -1) ) {
                                                        mat[seg.i][seg.j] = mat[p1.i][p1.j] +1;
                                                        pacpos.push(seg);
                                }       }       }
                        }
                }

                return bfs_fant(p);

}

inline Dir bfs_pacpill( Pos p) {

                for(int x = 0; x < 4; ++x) if ( (( cell(dest(p, desp[x])).type ) == Pill) or  (( cell(dest(p, desp[x])).type ) == Hammer)  ) return desp[x];

                queue<Pos> pacpos;
                bool found = false;

                mat[p.i][p.j] = 0; 

                pacpos.push( p );

                while( (not pacpos.empty()) and (not found)) {

                        Pos p1 = pacpos.front();

                        pacpos.pop();

                        int g = cell(p1).id;
                        if( (cell(p1).type == Hammer) or (cell(p1).type == Pill) or ( (g != -1) and robot(g).type != PacMan and robot(g).type == Ghost )) {
                                bool a = false;

                                for (int i=0; i < nb_ghosts() and (not a); ++i) if (g == ghost(me(), i).id) a = true;
                                if( not a ) {
                                found = true;

                                Pos p2;
                                while(mat[p1.i][p1.j] != 0 ) {

                                        for(int z = 0; z < 4; ++z) {
                                                p2 = dest(p1, desp[z]);
                                                if( mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1)p1 = p2;
                                                if( mat[p1.i][p1.j] == 0) {

                                                        if (desp[z] == Right) return Left ;
                                                        if (desp[z] == Left) return Right ;
                                                        if (desp[z] == Top) return Bottom ;
                                                        if (desp[z] == Bottom ) return Top ;
                                                }
                                        }
                                }

                                }
                        }
                        else {
                                for(int k = 0; k < 4; ++k) {
                                        if( movepac(p1,desp[k])) {
                                                Pos seg = dest(p1, desp[k]);

                                                if( pos_ok(seg) and (mat[seg.i][seg.j] == -1) ) {
                                                        mat[seg.i][seg.j] = mat[p1.i][p1.j] +1;
                                                        pacpos.push(seg);
                        }        }      }        }
                }
                for(int h = 0; h < 4; ++h) if( movepac( p, desp[h]) ) return desp[h];
                return None;

}

inline Dir bfs_fant( Pos p) {

                mat = vector<vector<int> >(rows(), vector<int>(cols(), -1));

                queue<Pos> pacpos;
                bool found = false;

                mat[p.i][p.j] = 0;

                pacpos.push( p );

                while( (not pacpos.empty()) and (not found)) {

                        Pos p1 = pacpos.front();

                        pacpos.pop();

                        int g = cell(p1).id;
                        if( (g != -1) and (robot(g).type == PacMan) and (robot(g).type != PowerPacMan) and (g != pacman(me()).id ) )  {

                                found = true;

                                Pos p2;
                                while(mat[p1.i][p1.j] != 0 ) {

                                        for(int z = 0; z < 4; ++z) {
                                                p2 = dest(p1, desp[z]);
                                                if( mat[p2.i][p2.j] == (mat[p1.i][p1.j]) - 1)p1 = p2;

                                                if( mat[p1.i][p1.j] == 0) {

                                                        if (desp[z] == Right) return Left ;
                                                        if (desp[z] == Left) return Right ;
                                                        if (desp[z] == Top) return Bottom ;
                                                        if (desp[z] == Bottom ) return Top ;
                                        }        }
                                }

                        }
                        else {
                                for(int k = 0; k < 4; ++k) {
                                        if( moveghost(p1,desp[k])) {
                                                Pos seg = dest(p1, desp[k]);

                                                if( pos_ok(seg) and (mat[seg.i][seg.j] == -1) ) {
                                                        mat[seg.i][seg.j] = mat[p1.i][p1.j] +1;
                                                        pacpos.push(seg);
                        }       }       }       }
                }
                for(int h = 0; h < 4; ++h) if( moveghost( p, desp[h]) ) return desp[h];
                return None;

}
};
/**  * Do not modify the following line. */
RegisterPlayer(PLAYER_NAME);


        cpp


