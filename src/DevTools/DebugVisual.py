import os

path = os.environ['VS100COMNTOOLS'] + "..\\Packages\\Debugger\\autoexp.dat"
print path + "\n"

autoExpand = """; HappyEngine
he::Guid=$BUILTIN(GUID)
; /HappyEngine"""

visualizers = """; HappyEngine
;------------------------------------------------------------------------------
;  he::List
;------------------------------------------------------------------------------
; list is previewed with "[<size>](<elements>)".
; It has [size] and [capacity] children, followed by its elements.
; The other containers follow its example.
he::List<*>|he::PrimitiveList<*>|he::ObjectList<*>{
	preview (
		#(
			"[",
			$e.m_Size,
			"](",
			#array(
				expr: $e.m_Buffer[$i],
				size: $e.m_Size
			),
			")"
		)
	)

	children (
		#(
			#([size] : $e.m_Size),
			#([capacity] : $e.m_Capacity),
			#array(
				expr: $e.m_Buffer[$i],
				size: $e.m_Size
			)
		)
	)
}
; /HappyEngine"""

print "Reading...\n"
file = open(path, 'r')
outText = ""
isReadingHappyCode = False
for line in file:
    if isReadingHappyCode == False:
        if "; HappyEngine" in line:
            isReadingHappyCode = True
        else:
            outText += line
    elif "; /HappyEngine" in line:
        isReadingHappyCode = False
    if "[AutoExpand]" in line:
        outText += autoExpand;
    if "[Visualizer]" in line:
        outText += visualizers;
file.close()

print "Writing...\n"
file = open(path,'w')
file.write(outText)
file.close()

print "Done!\n"



    
    
