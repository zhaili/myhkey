function StartEmacs ()
   window = FindWindow("Emacs", nil)
   if window then
      ActivateWindow(window)
   else
      RunProcess("D:\\Emacs\\bin\\runemacs.exe");
   end
end

function StartTC ()
   RunProcess("d:\\totalcmd\\TOTALCMD.EXE /o")
end

RegHotkey("WIN+3", "StartEmacs")
RegHotkey("WIN+4", "StartTC")

-- StartEmacs()
-- StartTC()
