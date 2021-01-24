# -*- coding: utf-8 -*-
"""
Created on Sat Mar 30 17:45:47 2019

@author: Dell
"""
import matplotlib.pyplot as plt   
import copy as copy
##############################################
#--------------Final Project-----------------#
#              By: Evan Lydon                #
##############################################

########################################################################
#PROJECT DESCRIPTION: TAKES RECTANGULAR ARRAY (NESTED lIST/MATRIX) OF ANY 
#SIZE AND DETERMINES WHAT SYMMETRY, IF ANY, THE ARRAY HAS.  THE SYMMETRY 
#ELEMENTS THAT THE PROGRAM CHECKS FOR ARE MIRROR LINES OF SYMMETRY. AVAIILALBE 
#MIRROR LINES INCLUDE "HORIZONTAL","VERTICAL" AND "NORTH-WEST" AND "NORTH-EAST 
#DIAGONAL MIRROR LINES. MIRROR LINE SYMMETRY MEANS THAT THE ARRAY IS THE SAME ON
#BOTH SIDES OF THE LINE. THE PROGRAM ASSUMES THAT CHARACTERS IN THE ARRAY
#REPRESENT SQUARE TILES, WITH DIFFERENT CHARACTERS REPRESENTING DIFFERENT 
#COLORED TILES. THIS REPRESENTATION IS SHOWN USING MATPLOTLIB.imshow() ALONG
#WITH A LIST OF SYMMETRY ELEMENTS THE ARRAY CONTAINS.
########################################################################


#checks for the presence of a vertical mirror plane. Checks line by 
#line going down using a nested list. Returns error in the case of 
#1D list, so need protection against feeding it 1D list.
def Symmetry(T):
    
    #length of first nested list
    i=len(T[0])-1
    Vsym=True
    
    #loop through rows in nested list
    for row in T:
        s=0
        
        #loop through only half the nested list because we are comparing
        #two sides of a mirror line.
        while (i-s)>s:
            
            #if list elements are equal, continue loop 
            if row[s]==row[i-s]:
                s=s+1
                
            #if any array elements are unequal, end loop and return false
            #for vertical symmetry
            else:
                Vsym=False
                break
    return Vsym
        

#checks for the presence of a horizontal mirror plane. Checks line by 
#line going down using a nested list. Returns error in the case of 
#1D list, so need protection against feeding it 1D list.
def Symmetry2(T):
    tlen=len(T)-1       
    t=range(len(T)) 
    b=range(len(T[0])) 
    HSym=True    
    for i in t:
        for a in b: 
            if T[i][a]==T[tlen-i][a]:
                continue
            else:
                HSym=False
                break
    return HSym


#1D list symmetry. Checks for vertical mirror plane in 
#1D list. useful for determining diagonal mirror planes and
#for the case of 1D lists that are not squares.
def Symmetry1D(L):
    Lit=range(len(L))
    Llen=len(L)-1
    for k in Lit:
        if L[k]==L[Llen-k]:
            continue
        else:
            return False
    return True        

#Test to determine if the array is a square. If the array is not a square,
#then it cannot have diagonal symmetry.
def IsSquare(T):
    square=True
    height=len(T)
    for i in T:
        length=len(i)
        if length==height:
            continue
        else:
            square=False
            break
    return square

#takes an array and returns a new array that is the flipped version
#of that array. Useful so i don't have to write a new
#funciton for north-west diagonal symmetry and i can reuse 
#my northeast diagonal symmetry function and just flip the array.
#flipArray does not alter original array :v
def flipArray(T):
    width=len(T[0])
    w_it=range(int(width/2))
    #must copy list so original list is not altered, use deep copy 
    #for nested lists 
    copyT = copy.deepcopy(T)
    for i in copyT:
        iref=i.copy()
        for j in w_it:
            i[j]=i[width-1-j]
            i[width-1-j]=iref[j]
        del iref    
    return copyT


#checks for North East diagonal symmetry. Does this by appending 
#diagonal segments to 1D array and then checking the symmetry of 
#the 1D array.  If all of the diagonals are symmetrical then 
#the entire array has diagonal symmetry.
def Symmetry3(T):
        #dimension of array(L=W)       
        D=len(T[0])        
        #make interables of dimensions 
        D_it=range(D)
        #loop checks top left to top right
        for i in D_it:
            #create new list
            diag_list=list()
            #iterable i
            i_it=range(i+1)
            for j in i_it:
                diag_list.append(T[j][(D-1+j)-i])
            if Symmetry1D(diag_list):
                continue
            else:
                return False
        #new iterable for D so that the longest diagonal is not checked
        #twice 
        aD_it=range(D-1)
        #loop checks top right to bottom right
        for k in aD_it:
            diag_list=list()
            k_it=range(k+1)
            for h in k_it:
                diag_list.append(T[D-h-1][k-h])
            if Symmetry1D(diag_list):
                continue
            else:
                return False
        return True
        
#checks for North West diagonal symmetry by flipping the array 
#from left to right and then running Symmetry3 function which checks for north
#east diagonal symmetry. If the flipped array has north east diagonal
#symmetry then the non-flipped array must have north west diagonal symmetry.
def Symmetry4(T):
    """determines north west diagonal symmetry"""
    return Symmetry3(flipArray(T))

#need function for 1D lists so we don't get error
def depth(l):
   for i in l:
       #isinstance returns true if i is a list
       if isinstance(i,list):
           return False
       else:
           continue
   return True

#function to convert list into a matrix (nested list)
#this allows a 1D list to be printed with plt.imshow()
def printable(T):
    a=[[]]
    l=range(len(T))
    for i in l:
        a[0].append(T[i])
    return a
        
            
#master function that calls all other functions and outputs
#results to the user. Also prints a visual representation of the array
#using square tiles in a larger rectangle            
def arraySym(T):   
    if depth(T):
        plt.imshow(printable(T),cmap='gray')
        plt.show()
        print('\nThis array has a horizontal mirror plane.')
        if Symmetry1D(T):
            print('\nThis array has a vertical mirror plane.')
            return 
        else:
            return
    plt.imshow(T,cmap='gray')
    plt.show()
    if Symmetry(T):
        print('\nThis array has vertical symmetry.')
    if Symmetry2(T):
        print('\nThis array has horizontal symmetry.')
    if IsSquare(T)==True: 
        if Symmetry3(T):
            print('\nThis array has north east diagonal symmetry.')
        if Symmetry4(T):
            print('\nThis array has north west diagonal symmetry.')
            return  
    else:
        return
        
T=      [[0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [2,0,0,0,0,0,0,0,0,2]]
arraySym(T)           
T_diag=[[0,0,0,0,1],
        [0,0,0,1,0],
        [0,0,1,0,0],
        [0,1,0,0,0],
        [2,0,0,0,0]]     

arraySym(T_diag)
T1D=[1,0,0,1]

arraySym(T1D)

T_diag2=[[1,0,0],
         [0,2,0],
         [0,0,3]]

arraySym(T_diag2)
     
T= [[1,0,0,1],
    [1,0,0,1],
    [1,1,0,1]]                




T2= [[1,0,1,0,1,0,1],
     [2,0,2,0,2,0,2],
     [3,0,3,0,3,0,3]]



T3= [[1,0,0,0,1],
     [0,1,0,1,0],
     [0,0,1,0,0],
     [0,1,0,1,0],
     [1,0,0,0,1]]




T5=[[1,0],
    [0,1]]

T_smile=[[0,0,0,0,0,0,0,0,0,0],
         [0,0,1,0,0,0,0,1,0,0],
         [0,0,0,0,0,0,0,0,0,0],
         [0,1,0,0,0,0,0,0,1,0],
         [0,0,1,0,0,0,0,1,0,0],
         [0,0,0,1,1,1,1,0,0,0],
         [0,0,0,0,0,0,0,0,0,0]]
arraySym(T_smile)


T4= [[1,0,1,0,1,0],
     [1,0,1,0,1,0],
     [0,1,0,1,0,1],
     [1,0,1,0,1,0]]

arraySym(T_diag2)
arraySym(T5)
arraySym(T3)
arraySym(T2)
arraySym(T)

