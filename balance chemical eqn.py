# -*- coding: utf-8 -*-
"""
Created on Thu May 16 13:38:39 2019

@author: Dell
"""

#balance chem equation
import math
elements_list=[]
def Elements():
    elements='H,He,Li,Be,B,C,N,O,F,Ne,Na,Mg,Al,Si,P,S,Cl,Ar,K,Ca,Sc,Ti,V,Cr,Mn,Fe,Co,Ni,Cu,Zn,Ga,Ge,As,Se,Br,Kr,Rb,Sr,Y,Zr,Ag,'
    global elements_list
    pos=0
    for e in range(len(elements)):
        if elements[e] == ',' and e==1:
            elements_list.append(elements[pos:e])
            pos=e
        elif elements[e] == ',':
            elements_list.append(elements[pos+1:e])
            pos=e
        else:
            continue
Elements()               
def parse(string):                   
    global elements_list
    #create list of nums 0-9
    nums=['1','2','3','4','5','6','7','8','9','0']        
    x=range(len(string))
    #formula stores number of each element in dictionary
    formula={}
    for i in x:
        if i < (len(string)-1):
            tempstr=string[i:i+2]
        else:
            tempstr='Not in List'
        if tempstr in elements_list:
            if i==(len(string)-2) and formula.get(tempstr)==None:
                formula[tempstr]=1
            elif i==(len(string)-2) and formula.get(tempstr)!=None:
                formula[tempstr]=1
            elif string[i+2:i+4] in elements_list or string[i+2] in elements_list:
                formula[tempstr]=1
                i=i+1
            else:
                for j in range(i+2,len(string)):
                    if string[j] in nums and j==(len(string)-1):
                        formula[tempstr]=int(string[i+2::])
                        i=i+1
                        break
                    elif string[j] in nums and string[j+1] in nums:
                        continue
                    elif string[j] in nums and string[j+1] not in nums:
                        formula[tempstr]=int(string[i+2:j+1])
                        i=j
                        break
                    
                    
        elif string[i] in elements_list:
            if i==(len(string)-1) and formula.get(string[i])==None:
                formula[string[i]]=1
            elif i==(len(string)-1) and formula.get(string[i])!=None:
                formula[string[i]]=1
            elif string[i+1:i+3] in elements_list or string[i+1] in elements_list and formula.get(string[i])==None:
                formula[string[i]]=1
            elif string[i+1:i+3] in elements_list or string[i+1] in elements_list and formula.get(string[i])!=None:
                formula[string[i]]+=1
            else:
                for j in range(i+1,len(string)):
                    if string[j] in nums and j==(len(string)-1):
                        formula[string[i]]=int(string[i+1::])
                        i=i+1
                        break
                    elif string[j] in nums and string[j+1] in nums:
                        continue
                    elif string[j] in nums and string[j+1] not in nums:
                        formula[string[i]]=int(string[i+1:j+1])
                        i=j
                        break
                    
                   
        else:
            continue
    return formula

parse('2Ag')


#Parses chemical equations and returns dictionary of number of each type of
#atom in each molecule    
def ParseEquation(string):
    nums=['1','2','3','4','5','6','7','8','9','0']
    halves=string.split('=')
    reactants=halves[0]
    products=halves[1]
    r_list=reactants.split('+')
    p_list=products.split('+')
    r_dict={}
    p_dict={}
    for i in r_list:
        x=i.strip()
        if x[0] in nums and x[1] not in nums:
            r_dict[x[1::]]=parse(x)
            s=x[1::]+' stoichiometry'
            r_dict[s]=int(x[0])
        elif x[0] in nums and x[1] in nums:
            count=2
            while x[count] in nums:
                count+=1
            r_dict[x[count::]]=parse(x)
            s=x[count::]+' stoichiometry'
            r_dict[s]=int(x[0:count])
        else:
            r_dict[x]=parse(x)
            s=x+' stoichiometry'
            r_dict[s]=1            
    for j in p_list:
        y=j.strip()
        if y[0] in nums and y[1] not in nums:
            p_dict[y[1::]]=parse(y)
            s=y[1::]+' stoichiometry'
            p_dict[s]=int(y[0])
        elif y[0] in nums and y[1] in nums:
            count=2
            while y[count] in nums:
                count+=1
            r_dict[y[count::]]=parse(y)
            s=y[count::]+' stoichiometry'
            r_dict[s]=int(y[0:count])
        else:
            p_dict[y]=parse(y)
            s=y+' stoichiometry'
            p_dict[s]=1 
    return r_dict,p_dict
    
    
ParseEquation('21H2O = 21O2 +4H2+Fe')

def reverseParseEq(r,p):
    string=''
    for i,j in enumerate(r):
        if i==0:
            string=string+str(r[j+' stoichiometry'])+j
        elif i%2==0:
            string=string+'+'+str(r[j+' stoichiometry'])+j
    string=string+'='
    for x,y in enumerate(p):
        if x==0:
            string=string=string+str(p[y+' stoichiometry'])+y
        elif x%2==0:
            string=string=string+'+'+str(p[y+' stoichiometry'])+y
    return string
            
        

#returns total number of each atom in react and prod
def balance(string):
    r,p=ParseEquation(string)
    r_all_elements={}
    p_all_elements={}
    for count,x in enumerate(r):
        if count==0 or count%2==0:
            s=r[x+' stoichiometry']
            for i in r[x]:
                if i in r_all_elements:
                    r_all_elements[i]=r_all_elements[i]+(r[x][i]*s)
                else:
                    r_all_elements[i]=(s*r[x][i])
        else:
            continue
    for countb,y in enumerate(p):
        if countb==0 or countb%2==0:
            t=p[y+' stoichiometry']
            for j in p[y]:
                if j in p_all_elements:
                    p_all_elements[j]=p_all_elements[j]+(p[y][j]*t)
                else:
                    p_all_elements[j]=(t*p[y][j])
    return r_all_elements, p_all_elements
                


def balance2(eq):
    lst=[]
    for string in eq:
        rr,pp=balance(string)
        if rr==pp:
            return string
        else:                
            r,p=ParseEquation(string)
            r_all,p_all=balance(string)
            for x in r_all:
                if r_all[x]==p_all[x]:
                    continue
                elif r_all[x]>p_all[x]:
                    for h,i in enumerate(p):
                        if h==0 and x in p[i] or h%2==0 and x in p[i]:                       
                            dif=r_all[x]-p_all[x]
                            pcopy=p.copy()
                            pcopy[i+' stoichiometry']=pcopy[i+' stoichiometry']+(math.ceil(dif/p[i][x]))
                            newstr=reverseParseEq(r,pcopy)
                            lst.append(newstr)
                        else:
                            continue
                        
                elif r_all[x] < p_all[x]:
                    for k,j in enumerate(r):
                        if k==0 and x in r[j] or k%2==0 and x in r[j]:
                            dif=p_all[x]-r_all[x]
                            rcopy=r.copy()
                            rcopy[j+' stoichiometry']=rcopy[j+' stoichiometry']+(math.ceil(dif/r[j][x]))
                            newstr=reverseParseEq(rcopy,p)
                            lst.append(newstr)
                        else:
                            continue
                else:
                    continue
#    print(lst)
    return balance2(lst)
        

print(balance2(['Cr+O2=Cr2O3']))