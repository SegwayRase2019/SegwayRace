# SegwayRase

#概要  
==================
後期のチーム制作「セグウェイレース」開発用のリポジトリです。  
デバッグする時は、data/src下の.vscodeForDebugから.vscodeをdata/srcに出してから行ってください。  

#gitの使い方（一部） 
================== 
**初めてpullする手順**  
`git init`  
`git pull https:.........`  

**新しいブランチを作成する**  
`git checkout 作成元となるブランチ名`  
`git checkout -b 作成するブランチ名`  

**変更したファイルをステージング**  
`git add 拡張子含むファイル名`  

**コミット**  
`git commit`  

vimが開くので`i`を押して挿入モードにして記述。  
半角モードで`Esc`を押して`:wq`を入力して`Enter`すればok  

**初めてのpush**  
(リポジトリ毎初回だけ)  
`git remote add origin https:......`  
(ブランチ毎に初回だけ)  
`git push -u origin 作成したブランチ名`  

**2回目以降のpush**  
`git push`  

**プルリクエストを送る**  
作業中なら[WIP]を書いておくとよい  
githubサイトにて  

**ローカルのブランチはマージされたら削除してよい**  
`git branch -d 削除対象のブランチ名`  
