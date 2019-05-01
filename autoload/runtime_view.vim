
scriptencoding utf-8
if !exists('g:loaded_runtimeview')
    finish
endif

let s:save_cpo = &cpo
set cpo&vim

let s:this_dir = expand('<sfile>:p:h')
let g:runtime_view#server_port = 3210

function! runtime_view#runtimeview_start() abort
    let server_path = s:this_dir . '/../js/server.js '
    call jobstart('node ' . server_path . g:runtime_view#server_port)
    call rpcnotify(0, 'NeoVim#plugin', 'RuntimeView', 'factory')
    call runtime_view#pass_filetype()
    let g:runtime_view#server_port += 5
endfunction

function! runtime_view#pass_filetype() abort
    call rpcnotify(0, 'NeoVim#plugin', 'RuntimeView', 'Reload', &filetype)
endfunction

function! runtime_view#loadview_start() abort
    call rpcnotify(0, 'NeoVim#plugin', 'RuntimeView', 'factory')
    call rpcnotify(0, 'NeoVim#plugin', 'RuntimeView', 'LoadFile',
                \ &filetype, expand('%:p'))
endfunction

function! runtime_view#quit() abort
    call rpcnotify(0, 'NeoVim#plugin', 'RuntimeView', 'Finish')
endfunction

let &cpo = s:save_cpo
unlet s:save_cpo

