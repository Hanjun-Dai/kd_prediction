import sys
import cPickle as cp
import scipy
import scipy.io

if __name__ == '__main__':
    f = open('kmat-f-1-h-3.pkl', 'rb')
    d = cp.load(f)
    f.close()

    scipy.io.savemat('f-1-h-3.mat', d)
