
scriptencoding utf-8
if !exists('g:loaded_runtimeview')
    finish
endif

let s:save_cpo = &cpo
set cpo&vim

let s:this_dir = expand('<sfile>:p:h')
let g:runtime_view_server_already_started = 0

function! runtime_view#runtimeview_start() abort
    let server_path = s:this_dir . '/../js/server.js'
    if !g:runtime_view_server_already_started
        call jobstart('node ' . server_path)
        let g:runtime_view_server_already_started = 1
    endif
    call rpcnotify(0, 'plugin', 'RuntimeView', 'factory')
    call runtime_view#pass_filetype()
endfunction

function! runtime_view#pass_filetype() abort
    call rpcnotify(0, 'plugin', 'RuntimeView', 'Reload', &filetype)
endfunction

function! runtime_view#loadview_start() abort
    call rpcnotify(0, 'plugin', 'RuntimeView', 'factory')
    call rpcnotify(0, 'plugin', 'RuntimeView', 'LoadFile',
                \ &filetype, expand('%:p'))
endfunction

let &cpo = s:save_cpo
unlet s:save_cpo

