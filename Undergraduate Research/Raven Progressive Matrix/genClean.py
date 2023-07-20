import json
import os
import prolog1 as prolog
from natsort import natsort_keygen, ns
import sys
from itertools import combinations
from itertools import permutations
import more_itertools
from copy import deepcopy


natural_keys = natsort_keygen(key=lambda y: y.lower())
print("Print extras? (T or F)")
PAns = input()
if (PAns == "T") or (PAns == "t"):
    PrintFlag =  True
else:
    PrintFlag = False

# Opening the json file based on user input
print("Which Matrix Set(with \): ")
folder = input()

MatPath = os.getcwd() + '\Matrix_Sets' + folder + '\json'
RPMList  = os.listdir(MatPath)
RPMList.sort(key=natural_keys)

#Array of the files they answered wrong
AnsWrong = []

# Tracking how many files right and how many wrong
AmtCor = 0
AmtWro = 0
Tot = 0

# Global Arrays
#
#
#
# mat[Row][Col][Obj][Prop]
    # OBJ = 0 will always be the number of objects in the cell
    # Properties:
    #   0: ID       int
    #   1: Shape    string
    #   2: Color    string
    #   3: Size     string
    #   4: x        int
    #   5: y        int
    #
# AnsCho[Id][Obj][Prop]
    # OBJ = 0 will always be the number of objects in the cell
    # Properties:
    #   0: ID       int
    #   1: Shape    string
    #   2: Color    string
    #   3: Size     string
    #   4: x        int
    #   5: y        int
    #
# RowComp[CompType][Row][Col][Prop]
    # CompType:
    #   0: Similar Properties of Objects in the cell (SPC)
    #   1: Similar Properties of Objects in the same x,y position (SPXY)
    #   2: Track consistency of the number objects and amount (NumObjTrk)
    # Prop:
    #   0: Shape
    #   1: Color
    #   2: Size
    #
    # Prop and Col only used in CompType 0,1
    # For CompType 2:
    #   RowComp[2][Row] = Flag
    #
# SpPattern[PatType][Row][Col][ActPattern]
    # PatType:
    #   0: Amount
    #   1: Shape
    #   2: Color
    #   3: Size
    #
    # PatType 0 only focuses on the amount in each col per row so Col is not used
    # SpPattern[0][Row][ActPattern]
    #
# GePattern[PatType][Row][Col][ActPattern]
    # PatType:
    #   0: Shape
    #   1: Color
    #   2: Size
    #
# Assume TRUE(1) from the start
    # CellPropFl[prop]
    # Prop:
    #   0: Shape
    #   1: Color
    #   2: Size
    #
# Assume TRUE(1) from the start
    # RowPropFl[prop]
    # Prop:
    #   0: Shape
    #   1: Color
    #   2: Size
    #
# PosAns[AnsCho][Obj][Prop]
    # OBJ = 0 will always be the ID of the answer choice
    # Properties:
    #   0: ID       int
    #   1: Shape    string
    #   2: Color    string
    #   3: Size     string
    #   4: x        int
    #   5: y        int
    #
# Hypothesis[PatType][Level][Pattern]
    # PatType:
    #   0: Shape
    #   1: Color
    #   2: Size
    #

# Going through each file
for jName in RPMList:
    Tot += 1
    print(f"\n\nStarting {Tot}: {jName}")
    
    file_name = os.path.join(MatPath,jName)

    f = open(file_name, "r")
    data = json.loads(f.read())

    # array the matrix will be held in
    mat = [[]for row in range(len(data['series']))]

    # Printing out the info in the series based on rows then columns
    # Puts the objects into the "series object tracker" (SOT)
    for ser in data['series']:
        for col in ser['columns']:
            mat[ser['row'] - 1].append([])
            SOT = []
            TT = [[]]
            objNum = 0
            for obj in col['objects']:
                SOT.append([])
                for prop in obj:
                    SOT[objNum].append(obj[prop])
                TT.append(SOT[objNum])
                objNum += 1
            mat[ser['row'] - 1][col['column'] - 1] = TT
            mat[ser['row'] - 1][col['column'] - 1][0] = objNum

    # Array the answer choices will be held in
    AnsCho = [0 for Choi in range(len(data['answers']))]

    AmtOfAns = 0

    for ans in data['answers']:
        AOT = []
        TT = [[]]
        objNum = 0
        for obj in ans['objects']:
            AOT.append([])
            for prop in obj:
                AOT[objNum].append(obj[prop])
            TT.append(AOT[objNum])
            objNum += 1
        AnsCho[ans['id']] = TT
        AnsCho[ans['id']][0] = objNum
        AmtOfAns += 1

    CorrectAns = data['correct']

    print("*****BUILDING MATRIX*****")
    
    # Printing the matrix and answer choices
    # to make sure everything is in its correct spot
    for x in range(len(mat)):
        for y in range(len(mat[0])):
            if x == (len(mat) - 1) and y == (len(mat[0]) - 1):
                break
            for obj in mat[x][y]:
                print(f"pos {x},{y}: {obj}")
            print("\n")
        print("\n")
    print("\n")

    for x in range(AmtOfAns):
        for y in range(len(AnsCho[x])):
            print(f"ans {x}: {AnsCho[x][y]}")
        print("\n")


    # Array for comparing objects in the same row
    RowComp = [[0 for row in range((len(mat)))] for CompType in range(3)]

    # Array to keep track of the specific patterns seen in the matrix
    SpPattern = [[[] for row in range(len(mat))] for PatType in range(4)]


    # Array used to track if properties of all objects per cell are the same
    CellPropFl = [1 for prop in range(3)]

    # Array used to track if properties of all objects per row are the same
    RowPropComp = ["" for prop in range(3)]
    RowPropFl = [1 for prop in range(3)]

    print("\n\n\n\n")

    # Do this for how many rows there are
    for r in range(len(mat)):

        TT = []

        # Tracking if there is a consistent amount of Obj
        #   Assume there is to begin with
        # Also track the specific pattern of amounts in the row by columns
        RowComp[2][r] = 1
        NumObjTrk = 0
        AmtPat = [0 for Pats in range(3)]

        # Do this for how many rows there are
        for c in range(len(mat[0])):
            
            TempCellPat = [[]for PatType in range(3)]
            TempCol = []

            # If it is the last cell
            #   Skip it as that is what we are trying to find
            if c == (len(mat[0]) - 1) and r == (len(mat) - 1):
                continue
            
            # Comparing in the cell (SPC)
            # SPC[Prop]
            #   0: Shape
            #   1: Color
            #   2: Size
            SPC = [1 for prop in range(3)]
            
            FirObj = 1 # Flag to see if it is the first object in the cell
            
            CellPropComp = ["" for prop in range(3)]
            
            for obj in mat[r][c]:
                
                # Check to see if it is the number objects
                #   Compare the number of objects
                #   Put the amount of objects into the AmtPat Tracker
                #   Then skip this iteration
                if isinstance(obj,int):
                    # If it is also the first column
                    #   Have the number of objects in this cell be the ampount to compare the others by
                    if c == 0:
                        NumObjTrk = obj

                    # Else
                    #   Compare to see if there is the same amount of objects
                    #       If not then set RowComp[r][2] to 0
                    else:
                        if obj != NumObjTrk:
                            RowComp[2][r] = 0
                            
                    AmtPat[c] = obj
                    continue

                # Track the Shape, Color, and Size of the first object to compare with the rest
                if FirObj == 1:
                    for prop in range(3):
                        CellPropComp[prop] = obj[prop + 1]

                    FirObj = 0

                    #if this is the first object in the row as well then set track its properties
                    if c == 0 and r != (len(mat) - 1):
                        for prop in range(3):
                            RowPropComp[prop] = obj[prop + 1]


                # See if the properties of the current object match the property of the first object
                #   If so then ignore and keep going
                #   If not
                #       Set the RowPropFl for that property to 0 so that later we know
                #       that the not every row has keeps that property
                #       Next check if the properties of the current object is the same
                #       as the first object in the same cell as it may still be posssible
                #       that the objects in the same cell have the same properties even if
                #       the whole row does not
                #       If not
                #           Set SPC for that property to 0
                # Skip the last row as that is what we are trying to find
                if r != (len(mat) - 1):
                    for prop in range(3):
                        if obj[prop + 1] != RowPropComp[prop]:
                            RowPropFl[prop] = 0
                            if obj[prop + 1] != CellPropComp[prop]:
                                CellPropFl[prop] = 0
                                SPC[prop] = 0

                # Putting the obj property into the TempCellPat for the coresponding property
                # Skip the last row as that is what we are trying to find
                for prop in range(3):
                    TempCellPat[prop].append(obj[prop + 1])

            for PatType in range(1,4):
                SpPattern[PatType][r].append(TempCellPat[PatType - 1])
            TT.append(SPC)
            
        RowComp[0][r] = TT
        SpPattern[0][r] = AmtPat


    # Going through the answer choices and awarding points
    #   Based on which ones contains objects that match the properties we are looking for
    # The answer choice with the most points will be the one selected

    NumObj = 0
    # First seeing how many objects in the answer choices we are looking for
    # If there is the same amount of objects in the row
    #   Look at how many objects are in the last row
    if RowComp[2][2] == 1:
        NumObj = mat[len(mat) - 1][0][0]
    
    # Else
    #   Look at the specific patterns of amounts in the rows and use the last number of the pattern that best fits the last row
    #   by following the first 2 cells of the last row
    else:
        FoundFl = 0
        for r in range(len(mat) - 1):
            for c in range(len(mat[0]) - 1):
                if SpPattern[0][len(mat) - 1][c] != SpPattern[0][r][c]:
                    FoundFl = 0
                    break
                else:
                    FoundFl = 1
            if FoundFl == 1:
                NumObj = SpPattern[0][r][2]
                break
                    
    

    # Array of the possible answers it can be based off the number of objects in the choice alone
    #   If the choice doesn't have the correct amount of objects to begin with it is safe to assume it is incorrect
    PosAns = []
    AnsPoints = []
    ind = 0
    for Num in range(AmtOfAns):
        if AnsCho[Num][0] == NumObj:
            PosAns.append(AnsCho[Num])
            PosAns[ind][0] = Num
            ind += 1
            AnsPoints.append(1)

    # Checking if there are consistents properties in the rows
    #   Assign the property its respective variable if so based on the past objects in the row
    shape = ["" for Num in range(NumObj)]
    color = ["" for Num in range(NumObj)]
    size = ["" for Num in range(NumObj)]

    SkipFl = [0 for prop in range(3)]
    PatPropRow = [0 for prop in range(3)]
    SpPatFoundFl = [0 for prop in range(3)]

    # Look at the specific patterns of shape,color, and size in the rows and use the last prop of the specifc pattern that best fits the
    #   last row by following the first 2 cells of the last row
    for prop in range(1,4): 
        FoundFl = 0
        for r in range(len(mat) - 1):
            for c in range(len(mat[0]) - 1):
                for PatPos in range(len(SpPattern[prop][r][c])):
                    if SpPattern[prop][len(mat) - 1][c][PatPos] != SpPattern[prop][r][c][PatPos]:
                        FoundFl = 0
                        break
                    else:
                        FoundFl = 1
            if FoundFl == 1:
                PatPropRow[prop - 1] = r
                SpPatFoundFl[prop - 1] = 1
                break


    Lets = ["A","B","C","D","E","F"]
    # Array to keep track of the general patterns seen in the matrix
    GePattern = [[] for PatType in range(3)]

    PosLevs = [0 for i in range(3)]
    # Going through the patterns and finding the most general ones
    for prop in range(3):
        for r in range(len(SpPattern[prop + 1])):
            Row = []
            # Array to track the objects we already know and assigned a letter
            Known = ["?"]
            # Find the general pattern for each col then add it to GePattern
            for c in range(len(SpPattern[prop + 1][r])):
                Col = []
                for pat in range(len(SpPattern[prop + 1][r][c])):

                    for K in range(len(Known)):
                        if Known[K] == "?":
                            Known[K] = SpPattern[prop + 1][r][c][pat]
                        elif Known[K] != SpPattern[prop + 1][r][c][pat]:
                            continue
                        Known.append("?")
                        Col.append(Lets[K])
                        break
                    if PrintFlag:
                        print(SpPattern[prop + 1][r][c][pat])
                Row.append(Col)

            if PrintFlag:
                print(f"{r}: {Known}")

            ApFlag = 1
            for TR in GePattern[prop]:
                if TR == Row:
                    ApFlag = 0
                    break
            if ApFlag:
                GePattern[prop].append(Row)
                
    if PrintFlag:
        print("\n\n\n")
        for prop in GePattern:
            print(prop)
            print("\n")


    
    AnsGuess = -1


    print("\n\n*****BUILDING HYPOTHESIS TREE*****")







    
    print(f"\n\n\n\n\nTESTING:")
    #\n{PosLevs[0]}\n{PosLevs[1]}\n{PosLevs[2]}
    for prop in range(len(GePattern)):
        Known = []
        for c in range(len(GePattern[prop])):
            for a in GePattern[prop][c]:
                for b in a:
                    if b not in Known:
                        Known.append(b)
        PosLevs[prop] = len(Known) + 1
        







    
    
    Options = [[] for prop in range(3)]
    Options[0] = ["square","circle","hexagon","triangle","pentagon", "star"]
    Options[1] = ["red","blue","green","purple", "yellow","orange"]
    Options[2] = ["small","large"]
    Hypothesis = [[[] for i in range(PosLevs[prop])] for prop in range(3)]
    SpGenPat = [0 for prop in range(3)]
    
    for prop in range(len(Hypothesis)):
        print(f"\nPROP: {prop}")
            
        for Level in range(len(Hypothesis[prop]) - 1):
            pLevelFlag = 1
                
            LetTempCombo = []
            a = combinations(Lets, Level+1)
            for i in a:
                for batch in more_itertools.batched(list(i), Level+1):
                    LetTempCombo.append(batch)
                    
            PropTempCombo = []
            a = permutations(Options[prop], Level+1)
            for i in a:
                for batch in more_itertools.batched(list(i), Level+1):
                    PropTempCombo.append(batch)

            if PropTempCombo == []:
                break

            if PrintFlag:
                print(f"\nPROPTEMPCOMBO[{prop}]: {PropTempCombo}\n")
                
            for LetCombo in LetTempCombo:
                BreakFlag = 0
                
                if PrintFlag:
                    print(f"\LetCombo: {LetCombo}")
                    
                for PropCombo in PropTempCombo:
                    
                    if PrintFlag:
                        print(f"\nPropCombo: {PropCombo}")
                    
                    for r in range(len(GePattern[prop]) - 1):
                        CurPat = []
                        CurPat = deepcopy(GePattern[prop][r])
                        
                        if PrintFlag:
                            print(f"CurPat: {CurPat}")
                            
                        LetUsed = 0
                        for L in range(Level + 1):
                            SkipLetUsed = 0
                            for c in range(len(CurPat)):
                                for ind in range(len(CurPat[c])):
                                    if CurPat[c][ind] == LetCombo[L]:
                                        if PrintFlag:
                                            print(f"{CurPat[c][ind]} == {LetCombo[L]}")
                                        CurPat[c][ind] = PropCombo[L]
                                        if not SkipLetUsed:
                                            SkipLetUsed = 1
                                            LetUsed += 1
                        # If not all the letters were used
                        #   We have reached the highest Level
                        
                        if PrintFlag:
                            print(CurPat)
                            
                        if LetUsed != (Level + 1):
                            print(f"{LetUsed} != {Level + 1}")
                            print("BREAKING")
                            BreakFlag = 1
                            break

                        

                        # Checking to see if the pattern works across the whole matrix
                        # If it does
                        #   Add it to the hypothesis
                        AddToHypFlag = 0
                        for row in range(len(mat) - 1):
                            Known = ["?" for i in range(len(Lets))]
                            TestPat = deepcopy(CurPat)
                            for c in range(len(mat[row])):
                                for ind in range(len(TestPat[c])):
                                    # Replacing the variables with whatever the appropriate variable in the row
                                    if TestPat[c][ind] in Lets:
                                        LetInd = Lets.index(TestPat[c][ind])
                                        if Known[LetInd] == "?":
                                            Known[LetInd] = mat[row][c][ind + 1][prop + 1]    
                                        TestPat[c][ind] = Known[LetInd]
                                        
                                        """rep = TestPat[c][ind]
                                        for i in range(len(TestPat[c])):
                                            if TestPat[c][i] == rep:
                                                if PrintFlag:
                                                    print(TestPat[c][ind])
                                                    print(mat[row][c][ind + 1][prop + 1])
                                                TestPat[c][i] = mat[row][c][ind + 1][prop + 1]"""
                                        
                            # If the TestPat can be applied to the row
                            #   Increment AddToHypFlag
                            if TestPat == SpPattern[prop + 1][row]:
                                AddToHypFlag += 1

                        # Doing the same except for the most general patterns
                        # Putting them on the lowest level: 0
                        if (Level == 0) and (GePattern[prop][r] not in Hypothesis[prop][Level]):
                            AddToHypFlagLev0 = 0
                            for row in range(len(mat) - 1):
                                Known = ["?" for i in range(len(Lets))]
                                TestPat = deepcopy(GePattern[prop][r])
                                for c in range(len(mat[row])):
                                    for ind in range(len(TestPat[c])):
                                        # Replacing the variables with whatever the appropriate variable in the row
                                        if TestPat[c][ind] in Lets:
                                            LetInd = Lets.index(TestPat[c][ind])
                                            if Known[LetInd] == "?":
                                                Known[LetInd] = mat[row][c][ind + 1][prop + 1]    
                                            TestPat[c][ind] = Known[LetInd]
                                            
                                            """rep = TestPat[c][ind]
                                            for i in range(len(TestPat[c])):
                                                if TestPat[c][i] == rep:
                                                    if PrintFlag:
                                                        print(TestPat[c][ind])
                                                        print(mat[row][c][ind + 1][prop + 1])
                                                    TestPat[c][i] = mat[row][c][ind + 1][prop + 1]"""
                                            
                                # If the TestPat can be applied to the row
                                #   Increment AddToHypFlag
                                if TestPat == SpPattern[prop + 1][row]:
                                    AddToHypFlagLev0 += 1
                                    
                            if (AddToHypFlagLev0 == (len(mat) - 1)):
                                print(f"\nLevel: {Level}")
                                print(f"Added: {GePattern[prop][r]}\n")
                                Hypothesis[prop][Level].append(deepcopy(GePattern[prop][r]))
                                
                        if pLevelFlag:
                            print(f"\nLevel: {Level + 1}")
                            pLevelFlag = 0
                                   
                        # If the pattern can be applied to every row and is not already in the hypothesis
                        #    Add the pattern to the hypothesis at the appropriate level
                        if (AddToHypFlag == (len(mat) - 1)) and (CurPat not in Hypothesis[prop][Level + 1]):
                            print(f"Added: {CurPat}\n")
                            Hypothesis[prop][Level + 1].append(CurPat)
                                
                    if BreakFlag:
                        if PrintFlag:
                            print("BREAKING 2 ")
                        break
                if BreakFlag:
                    if PrintFlag:
                        print("BREAKING 3")
                    continue
            """if BreakFlag:
                if PrintFlag:
                    print("BREAKING 4")
                continue"""
        print("\n")

    print(f"\n\n\n\nGePattern: {GePattern}")
    print(f"\n\nHypothesis: {Hypothesis}\n\n")


    for prop in range(len(Hypothesis)):
        for lev in range(len(Hypothesis[prop])):
            for el in Hypothesis[prop][lev]:
                top = lev
                if PrintFlag:
                    print(f"prop {prop}, level {lev}: {el}")
        SpGenPat[prop] = Hypothesis[prop][top][0]
        print(f"\nTop of prop {prop} at level {top}:{SpGenPat[prop] }\n")



    print("\n\n\n\n*****FINDING ANSWER*****")
    # Finding the answer
    #   We make temporary rows consisting of the last row in the matrix with the answer choices subbed into the last cell
    #   Apply the most specific pattern from each prop to the temp row
    #   If all patterns apply to that row then we can assume it has the correct answer in it
    for CurAns in PosAns:
        CorAnsFlag = 0
        TempRow = deepcopy(mat[len(mat) - 1])
        TempRow.append(CurAns)
        print(f"\n\nTesting Choice {CurAns[0]}: {TempRow}\n")
        for prop in range(len(SpGenPat)):

            Known = ["?" for i in range(len(Lets))]
            
            TestPat = deepcopy(SpGenPat[prop])
            print(f"Org TestPat: {TestPat}")
            for c in range(len(TempRow)):
                for ind in range(len(TestPat[c])):
                    # Replacing the variables with whatever the appropriate variable in the row
                    if TestPat[c][ind] in Lets:
                        LetInd = Lets.index(TestPat[c][ind])
                        if Known[LetInd] == "?":
                            Known[LetInd] = TempRow[c][ind + 1][prop + 1]    
                        TestPat[c][ind] = Known[LetInd]
                    
                        """rep = TestPat[c][ind]
                        for i in range(len(TestPat[c])):
                            if TestPat[c][i] == rep:
                                if PrintFlag:
                                    print(TestPat[c][i])
                                    print(TempRow[c][ind + 1][prop + 1])
                                TestPat[c][i] = TempRow[c][ind + 1][prop + 1]"""
                                
            print(f"UpDated TestPat: {TestPat}\n")
            # If the TestPat can be applied to the row
            #   Increment CorAnsFlag
            PropWorkFlag = 1
            for c in range(len(TempRow)):
                Comp = []
                for obj in TempRow[c]:
                    if isinstance(obj,int):
                        continue
                    Comp.append(obj[prop + 1])

                if PrintFlag:
                    print(f"Comparing prop {prop} at Col {c}:")
                    print(f"TestPat: {TestPat[c]}")
                    print(f"TempRow: {Comp}\n")

                if TestPat[c] != Comp:
                    PropWorkFlag = 0
                    break

            if PropWorkFlag:
                CorAnsFlag += 1

            else:
                break
            
        if not PropWorkFlag:
            continue
        
        
        if CorAnsFlag == len(SpGenPat):
            print("CHOICE FOUND")
            AnsGuess = CurAns[0]
            break

                   

    # Compare the answer to the correct choice and see if it is correct
    if AnsGuess == CorrectAns:
        print("\n***CORRECT***")
        print(f"It is {AnsCho[AnsGuess][0]}: {AnsCho[AnsGuess][1:]}")
        AmtCor += 1
        
    else:
        print(f"\nWRONG :'(")
        WrongStatement = jName + " guessed " + str(AnsGuess) + " when correct is " + str(CorrectAns)
        AnsWrong.append(WrongStatement)
        AmtWro += 1

    print(f"\nFinished {Tot}: {jName}\n\n")
    f.close()
    EndInput = input()
    if EndInput == "E" or EndInput ==  "e":

        print(f"{AmtCor} correct out of {Tot}")
        print(f"{AmtWro} wrong out of {Tot}")
        print("Files Wrong:")
        for f in AnsWrong:
            print(f)
        print("\n\n")
        sys.exit()


print(f"{AmtCor} correct out of {Tot}")
print(f"{AmtWro} wrong out of {Tot}")
print("\nFiles Wrong:")
for f in AnsWrong:
    print(f)

print("\n\n")
