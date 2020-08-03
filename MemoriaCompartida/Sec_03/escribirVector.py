#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jun 13 12:55:04 2020

@author: Rodolfo Escobar
"""

import numpy as np

N = 10

v = np.random.uniform(0,100,N)

#-- numpyarray -> Arreglo de bytes --
v_bytes = v.tobytes('C')

#-- Concatenación de bytes (N v[0]...v[N-1]) --
Dimbyte = np.int32(N).tobytes('C')
Data_bytes = Dimbyte + v_bytes

#-- Escribir archivo de vector --
f = open("Vector.dat", "wb")
f.write(Data_bytes)
f.close()
