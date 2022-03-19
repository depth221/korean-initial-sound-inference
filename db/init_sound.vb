Dim blnChk As Boolean
Dim Str1st() As String
Dim Str2nd() As String
Dim Str3rd() As String

 

Private Sub strBase()
Dim strFirst As String, strMiddle As String, strFinal As String
    strFirst = "ㄱ ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ"
    strMiddle = "ㅏ ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ"
    strFinal = " ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ" & _
      " ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ"
    Str1st = Split(strFirst)
    Str2nd = Split(strMiddle)
    Str3rd = Split(strFinal)
End Sub


Private Function StrtoPhon(ByVal strKor As String) As String
Dim lngUnicode As Long, lngCode As Long, first As String
Dim middle As String, last As String
    lngUnicode = AscW(strKor)
    If lngUnicode < &HAC00 Or lngUnicode > &HD7A3 Then
      StrtoPhon = strKor
    Else
      lngCode = lngUnicode - &HAC00
      first = Str1st(Int(lngCode / (21 * 28)))
      lngCode = lngCode Mod (21 * 28)
      middle = Str2nd(Int(lngCode / 28))
      last = Str3rd(lngCode Mod 28)
      StrtoPhon = first & middle & last
    End If
End Function

 

Function STRTOSYLLABLE(ByVal strKorean As String) As String
Dim i As Long, j As Long, strTemp As String, strX() As String
    If Not blnChk Then strBase
    j = Len(strKorean)
    ReDim strX(1 To j)
    For i = 1 To j
      strTemp = Mid$(strKorean, i, 1)
      strTemp = StrtoPhon(strTemp)
      strX(i) = strTemp
    Next i
    STRTOSYLLABLE = Join(strX, "")
End Function
