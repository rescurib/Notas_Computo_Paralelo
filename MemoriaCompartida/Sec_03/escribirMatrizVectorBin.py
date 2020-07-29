#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jun 13 12:55:04 2020

@author: Rodolfo Escobar
"""

import numpy as np

n = 2000
m = 2000

A = np.random.rand(n,m) 
v = np.random.rand(m)

#-- numpyarray -> Arreglo de bytes --
A_bytes = A.tobytes('C')
v_bytes = v.tobytes('C')

#-- Concatenación de bytes (n m A[0][0]...A[m][n])
Dimbyte = np.int32(n).tobytes('C') + np.int32(m).tobytes('C')
Data_bytes = Dimbyte + A_bytes

#-- Escribir archivo de matriz --
f = open("Matriz.dat", "wb")
f.write(Data_bytes)
f.close()

#-- Concatenación de bytes (m v[0]...v[m]) --
Dimbyte = np.int32(m).tobytes('C')
Data_bytes = Dimbyte + v_bytes

#-- Escribir archivo de vector --
f = open("Vector.dat", "wb")
f.write(Data_bytes)
f.close()
