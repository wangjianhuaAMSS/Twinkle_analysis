#ifndef _RESIZESTATE_H_
#define _RESIZESTATE_H_





template<typename T>
vector<vector<vector<T>>> vector_2_cube(vector<T> state, size_t x, size_t y, size_t z) {
    assert (state.size() == x * y * z);
    vector<vector<vector<T>>> cube_state;
    for (size_t i = 0; i < x; i++) {
        vector<vector<T>> tmp(y);
        for (size_t j = 0; j < y; j++) {
            for (size_t k =0; k < z; k++) {
                tmp[j].push_back(state[i * y * z + j * z + k]);
            }
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}




template<typename T>
vector<T> cube_2_vector(vector<vector<vector<T>>> cube_state) {
    vector<T> state;
    for (size_t i = 0; i < cube_state.size(); i++) {
        for (size_t j = 0; j < cube_state[0].size(); j++) {
            for (size_t k =0; k < cube_state[0][0].size(); k++) {
                state.push_back(cube_state[i][j][k]);
            }
        }
    }
    return(state);
}




template<typename T>
vector<vector<vector<T>>> cube_2_cube(vector<vector<vector<T>>> cube_state1, size_t x, size_t y, size_t z) {
    assert (cube_state1.size() * cube_state1[0].size() * cube_state1[0][0].size() == x * y * z);
    vector<vector<vector<T>>> cube_state2; 
    vector<T> state;
    state = cube_2_vector<T>(cube_state1);
    cube_state2 = vector_2_cube<T>(state, x, y, z);
    return(cube_state2);
}




template<typename T>
vector<vector<T>> vector_2_plane(vector<T> state, size_t x, size_t y) {
    assert (state.size() == x * y);
    vector<vector<T>> plane_state;
    for (size_t i = 0; i < x; i++) {
        vector<T> tmp;
        for (size_t j = 0; j < y; j++) {
            tmp.push_back(state[i * y+ j]);
        }
        plane_state.push_back(tmp);
    }
    return(plane_state);
}



template<typename T>
vector<T> plane_2_vector(vector<vector<T>> plane_state) {
    vector<T> state;
    for (size_t i = 0; i < plane_state.size(); i++) {
        for (size_t j = 0; j < plane_state[0].size(); j++) {
            state.push_back(plane_state[i][j]);
        }
    }
    return(state);
}



template<typename T>
vector<vector<T>> plane_2_plane(vector<vector<T>> plane1, size_t x, size_t y) {
    assert (plane1.size() * plane1[0].size() == x * y);
    vector<vector<T>> plane2;
    vector<T> state;
    state = plane_2_vector<T>(plane1);
    plane2 = vector_2_plane<T>(state, x, y);
    return(plane2);
}



template<typename T>
vector<vector<T>> xs_of_plane(vector<vector<T>> plane_state) {
    vector<vector<T>> xs;
    for (size_t j = 0; j < plane_state[0].size(); j++) {
        vector<T> tmp;
        for (size_t i = 0; i < plane_state.size(); i++) {
            tmp.push_back(plane_state[i][j]);
        }
        xs.push_back(tmp);
    }
    return(xs);
}


template<typename T>
vector<vector<T>> ys_of_plane(vector<vector<T>> plane_state) {
    return(plane_state);
}

template<typename T>
vector<vector<T>> plane_xs_2_plane(vector<vector<T>> xs) {
    vector<vector<T>> plane_state;
    for (size_t j = 0; j < xs[0].size(); j++) {
        vector<T> tmp;
        for (size_t i = 0; i < xs.size(); i++) {
            tmp.push_back(xs[i][j]);
        }
        plane_state.push_back(tmp);
    }
    return(plane_state);
}

template<typename T>
vector<vector<T>> plane_ys_2_plane(vector<vector<T>> ys) {
    return(ys);
}


template<typename T>
vector<vector<T>> ys_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> ys;
    for (size_t i = 0; i < cube_state.size(); i++) {
        for (size_t k =0; k < cube_state[i][0].size(); k++) {
            vector<T> tmp;
            for (size_t j =0; j < cube_state[i].size(); j++) {
                tmp.push_back(cube_state[i][j][k]);
            }
            ys.push_back(tmp);
        }
    }
    return(ys);
}



template<typename T>
vector<vector<vector<T>>> cube_ys_2_cube(vector<vector<T>> ys, size_t x, size_t z) {
    vector<vector<vector<T>>> cube_state;
    assert (ys.size() == x * z); 
    for (size_t i = 0; i < x; i++) { 
        vector<vector<T>> tmp(ys[0].size(), vector<T>(z));
        for (size_t k =0; k < z; k++) {
            for (size_t j =0; j < ys[0].size(); j++) {
                tmp[j][k] = ys[z * i + k][j];
            }
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}



template<typename T>
vector<vector<T>> xs_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> xs;
    for (size_t j = 0; j < cube_state[0].size(); j++) {
        for (size_t k =0; k < cube_state[0][0].size(); k++) {
            vector<T> tmp;
            for (size_t i =0; i < cube_state.size(); i++) {
                tmp.push_back(cube_state[i][j][k]);
            }
            xs.push_back(tmp);
        }
    }
    return(xs);
}



template<typename T>
vector<vector<vector<T>>> cube_xs_2_cube(vector<vector<T>> xs, size_t y, size_t z) {
    vector<vector<vector<T>>> cube_state;
    assert(xs.size() == y * z);
    for (size_t i = 0; i < xs[0].size(); i++) { 
        vector<vector<T>> tmp(y, vector<T>(z));
        for (size_t k =0; k < z; k++) {
            for (size_t j =0; j < y; j++) {
                tmp[j][k] = xs[z * j + k][i];
            }
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}



template<typename T>
vector<vector<T>> zs_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> zs;
    for (size_t i = 0; i < cube_state.size(); i++) {
        for (size_t j =0; j < cube_state[0].size(); j++) {
            vector<T> tmp;
            for (size_t k =0; k < cube_state[0][0].size(); k++) {
                tmp.push_back(cube_state[i][j][k]);
            }
            zs.push_back(tmp);
        }
    }
    return(zs);
}


template<typename T>
vector<vector<vector<T>>> cube_zs_2_cube(vector<vector<T>> zs, size_t x, size_t y) {
    vector<vector<vector<T>>> cube_state;
    assert(zs.size() == x * y);
    for (size_t i = 0; i < x; i++) { 
        vector<vector<T>> tmp(y, vector<T>(zs[0].size()));
        for (size_t k =0; k < zs[0].size(); k++) {
            for (size_t j =0; j < y; j++) {
                tmp[j][k] = zs[i * y + j][k];
            }
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}



template<typename T>
vector<vector<T>> xy_planes_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> xyps;
    for (size_t k = 0; k < cube_state[0][0].size(); k++) {
        vector<T> tmp;
        for (size_t i = 0; i < cube_state.size(); i++) {   
            for (size_t j =0; j < cube_state[0].size(); j++) {
                tmp.push_back(cube_state[i][j][k]);
            }    
        }
        xyps.push_back(tmp);
    }
    return(xyps);
}



template<typename T>
vector<vector<vector<T>>> cube_xy_planes_2_cube(vector<vector<T>> xyps, size_t x, size_t y) {
    vector<vector<vector<T>>> cube_state;
    assert(xyps[0].size() == x * y);
    for (size_t i = 0; i < x; i++) {
        vector<vector<T>> tmp(y, vector<T>(xyps.size()));
        for (size_t j = 0; j < y; j++) {   
            for (size_t k =0; k < xyps.size(); k++) {
                tmp[j][k] = xyps[k][i * y + j];
            }    
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}


template<typename T>
vector<vector<T>> xz_planes_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> xzps;
    for (size_t j = 0; j < cube_state[0].size(); j++) {
        vector<T> tmp;
        for (size_t i = 0; i < cube_state.size(); i++) {   
            for (size_t k =0; k < cube_state[0][0].size(); k++) {
                tmp.push_back(cube_state[i][j][k]);
            }   
        }
        xzps.push_back(tmp);
    }
    return(xzps);
}



template<typename T>
vector<vector<vector<T>>> cube_xz_planes_2_cube(vector<vector<T>> xzps, size_t x, size_t z) {
    vector<vector<vector<T>>> cube_state;
    assert(xzps[0].size() == x * z);
    for (size_t i = 0; i < x; i++) {
        vector<vector<T>> tmp(xzps.size(), vector<T>(z));
        for (size_t j = 0; j < xzps.size(); j++) {   
            for (size_t k =0; k < z; k++) {
                tmp[j][k] = xzps[j][i * z + k];
            }    
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}



template<typename T>
vector<vector<T>> yz_planes_of_cube(vector<vector<vector<T>>> cube_state) {
    vector<vector<T>> yzps;
    for (size_t i = 0; i < cube_state.size(); i++) {
        vector<T> tmp;
        for (size_t j = 0; j < cube_state[0].size(); j++) {   
            for (size_t k =0; k < cube_state[0][0].size(); k++) {
                tmp.push_back(cube_state[i][j][k]);
            }    
        }
        yzps.push_back(tmp);
    }
    return(yzps);
}



template<typename T>
vector<vector<vector<T>>> cube_yz_planes_2_cube(vector<vector<T>> yzps, size_t y, size_t z) {
    vector<vector<vector<T>>> cube_state;
    assert(yzps[0].size() == y * z);
    for (size_t i = 0; i < yzps.size(); i++) {
        vector<vector<T>> tmp(y, vector<T>(z));
        for (size_t j = 0; j < y; j++) {   
            for (size_t k =0; k < z; k++) {
                tmp[j][k] = yzps[i][j * z + k];
            }    
        }
        cube_state.push_back(tmp);
    }
    return(cube_state);
}



template<typename T>
void cout_plane(vector<vector<T>> state) {
    for (size_t i = 0; i < state.size(); i++) {
        for (size_t j = 0; j < state[i].size(); j++) {
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}



template<typename T>
void cout_cube(vector<vector<vector<T>>> state) {
    for (size_t i = 0; i < state.size(); i++) {
        cout << i << endl;
        for (size_t j = 0; j < state[i].size(); j++) {
            for (size_t k =0; k < state[i][j].size(); k++) {
                cout << state[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    } 
}
#endif