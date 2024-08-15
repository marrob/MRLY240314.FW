# -*- coding: utf-8 -*-
"""
Created on Fri Feb 17 08:56:52 s2023

@author: marrob
"""

import subprocess

subprocess.run([
                 r"C:\Program Files (x86)\teraterm\ttermpro.exe",
                 "/BAUD=230400",                    #Speed
                 "/C=9",                            #Serial port COMx    
                 "/DS",                             #Disable displaying the "New connection" dialog on startup
                 "/W=Start by Python subprocess"    #Window title                
                 ])
