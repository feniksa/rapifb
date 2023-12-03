nnoremap <F8> :make<CR>
set makeprg=ninja\ -C\ build

nnoremap <F9> :Termdebug -cd=build/tool --args ./tools/rapishow --n 5<CR>
set path+=fb
set path+=tools

