# NCtfU Official Writeup | HITCON-2020
###### tags: `nctfu` `hitcon`

[TOC]


## 0x0: 故事背景

![攤位精美白板](https://i.imgur.com/7uwtdda.jpg)

<img src="https://i.imgur.com/FqtCyro.jpg" width=49%> <img src="https://i.imgur.com/IAfZUH9.jpg" width=49%>


## 0x1: Crypto

- 筆記本最後一頁，會找到一推看不懂的密文，以及一張 6 x 6 的 table。

![](https://i.imgur.com/o2VuX6O.jpg)

- 稍微觀察一下，會發現這些數字都是介於 0 ~ 5 之間，因此推測應該是跟 row 和 column 的 index 有關。

- `"hao.kang."` 似乎是一部分解開的明文，或許可以從這邊推敲看看加密的邏輯。
    - 假設每組數字代表的是 `(row)(col)`，那麼 `05 10 32` 就會得到 `x e 8`。
    - 假設每組數字代表的是 `(col)(row)`，那麼對應下來則是 `s l z`。
    - 全部轉成 ascii code，會得到：
        ```
        "hao" -> 104,  97, 111
        "xe8" -> 120, 101,  56  (X)
        "slz" -> 115, 108, 122  (O)
        ```
    - 觀察字母之間的間距，會發現 `"hao"` 平移 `+11` 後會變成 `"slz"`。
    - 用 `"kang"` 驗證一下，會發現這個推論是對的：
        ```
        "kang" -> 107, 97, 110, 103
        "vlyr" -> 118, 108, 121, 114
        ```

- 打星星的部分似乎是重點，直接對應下來，會得到：
    ```
    140.115.59.92.azce.8000
    =>  IP   = 140.115.59.92
        PORT = 8000
    ```
    - 好像滿多人卡在這邊，因為中間夾了一串 `"azce"`，直接連會連不上。
        > 這部分我們可能設計得不太好，因為凱撒加密預設是只處理英文的部分，不處理數字。因此，最後就變成了英文做 rotation 但數字不做，確實滿令人困惑的 :sweat_smile:
        > 然後還有一個雷點，就是某人一開始寫明文訊息寫得很開心，一不小心就生出長度為 93 的密文了XDD 還好有學長們幫忙 debug :pray:
    

## 0x2: Web

- 發現了一個登入頁，但沒有帳密。

![](https://i.imgur.com/M2soscU.png)

- 跑一下 dirsearch，就發現有 `robots.txt`。

![](https://i.imgur.com/Lo2yL6u.png)

- 訪問 `robots.txt` 後，就得到一行字：
    ```
    Disallow: /53cR37
    ```
- 把 `53cR37` 載下來後，得到一組帳密 `artis / cut3`。

![](https://i.imgur.com/0h3OfeG.png)

- 登入了以後，就可以看到聊天紀錄了。不過最後兩則訊息卻 `[已被刪除]`。

![](https://i.imgur.com/qHO1Sbx.png)

- 如果按右上角的藍色 Profile，會進到 "artis" 的個人頁面。
- 如果是點聊天室中 "小辣椒" 的名字，則會進到 "小辣椒" 的個人頁。

![](https://i.imgur.com/EejPpmb.png)
![](https://i.imgur.com/GnwYLxE.png)

- 從上面可以發現，只有 `小辣椒` 是 Admin 帳戶類型。
    - 如果用 `小辣椒` 的身份登入的話，好像就有機會看到被刪除的訊息了(?

- 看一下 HTML，會發現 LOG IN 按鈕是連到 `login.php`。

![](https://i.imgur.com/5xAw30O.png)

- 直接連 `login.php`，就得到 source code 了！:open_mouth: 

![](https://i.imgur.com/PB1rLaq.png)

- 稍微研究一下，會發現 `$sql` 那行有 injection 漏洞。
    - 前提是不能撈太多，只能撈出一行來。
    - 如果用 `小辣椒' -- `，那麼整句就會變成：
        ```sql
        SELECT ... WHERE username='小辣椒' -- ' and password=''
        ```
    - 這樣一來，就只會撈出一行來了。
        > 這邊還有一個小雷點，就是 token 在還沒解開前好像就會跳出來XD 看來是測的不夠周全~

![](https://i.imgur.com/TNyFtsi.png)

<br>

> 進階挑戰：使用 UNION 在網址列撈資料
> - 因為個人頁面的網址列是 `?name=artis`，因此或許會有 SQLI 漏洞 :thinking_face:
> 
> - 使用 `artis' UNION SELECT 1,2,3,4,5,6 -- '` 可以得知 query 結果的寬度 = 6。
> ![](https://i.imgur.com/RYNBfse.png)
> 
> - 透過 INFORMATION_SCHEMA，可以得知訊息是 message 表當中的 msg 欄位。
> - 使用 `artis' UNION SELECT 1,2,3,4,5,msg FROM message -- '` 即可撈到資料。
> ![](https://i.imgur.com/izbNntG.png)


## 0x3: Pwn (Misc ?)
> [name=Yen]

* 接續上一關拿到的網址，進來會看到只有一個input

    ![](https://i.imgur.com/Olbkwrm.jpg)

* 隨便輸入一點東西，會看到BOW的字樣，其實就是要玩家輸入BufferOverflow的垃圾padding

    ![](https://i.imgur.com/ojZFXbW.jpg)

* 例如說，輸入60字會拿到**N**

    ![](https://i.imgur.com/bfd3lXF.jpg)

* 最後全拿完會拿到**NCtfU**

    ![](https://i.imgur.com/WYGY9YL.jpg)

* 輸入進去就可以到下一關了XD

    ![](https://i.imgur.com/mCxspOa.jpg)


## 0x4: Stego
> [name=Artis24106]
- 由上一關最後的網址 `https://drive.google.com/file/d/1OC5I9xM1KojHpYTuJKw-icUcFHL1p6UY/view` 可以得到圖片 `qrcode.png`
    ![](https://i.imgur.com/AGXT7XT.png)

- 用手機掃描或是使用線上工具（如 `https://zxing.org/w/decode`）可以得到 qrcode 訊息 `There is something 'in' the picture. Hint1: strings`
    ![](https://i.imgur.com/6eK8FKO.png)

- 提示1為 `strings`。它是一個指令，會把檔案中的 printable characters 印出來。
- 於是使用 `strings qrcode.png` 可以在最下面看到 `Hint2: zsteg`
    ```
    ...
    go{^
    VY-Z
    @HXi
    i+1U
    bzEO
    y`be
    7``O
    IEND
    Hint2: zsteg
    ```
- 提示2為 `zsteg`。它是一個圖片隱寫檢測工具（[連結](https://ctf-wiki.github.io/ctf-wiki/misc/picture/png-zh/)），可以找出由[LSB](https://ctf-wiki.github.io/ctf-wiki/misc/picture/png-zh/)隱寫在圖片內的資訊。
- 使用 `zsteg qrcode.png` 得到下一階段的網址
    ![](https://i.imgur.com/irNjADb.png)


## 0xA: Scoreboard

![](https://i.imgur.com/XEf5Dxx.png)

![](https://i.imgur.com/Ao4LCXP.png)
