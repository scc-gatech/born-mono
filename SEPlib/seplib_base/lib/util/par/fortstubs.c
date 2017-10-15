#define f2cFortran
#include "cfortran.h"
#include <sep_par.h>
FCALLSCSUB1(sep_close_lock,SEP_CLOSE_LOCK_,sep_close_lock_,STRING)
FCALLSCSUB1(sep_open_lock,SEP_OPEN_LOCK_,sep_open_lock_,STRING)
FCALLSCSUB1(sep_close_lock,SEP_CLOSE_LOCK,sep_close_lock,STRING)
FCALLSCSUB1(sep_open_lock,SEP_OPEN_LOCK,sep_open_lock,STRING)
#ifdef SEP_MPI
FCALLSCFUN0(INT,mpi_sep_receive_args,MPI_SEP_RECEIVE_ARGS_,mpi_sep_receive_args_)
FCALLSCFUN3(INT,mpi_sep_send_args,MPI_SEP_SEND_ARGS_,mpi_sep_send_args_,INT,INT,INT)
FCALLSCFUN0(INT,mpi_sep_pass_args,MPI_SEP_PASS_ARGS_,mpi_sep_pass_args_)
FCALLSCFUN0(INT,mpi_sep_pass_args,MPI_SEP_PASS_ARGS,mpi_sep_pass_args)
FCALLSCFUN0(INT,mpi_sep_receive_args,MPI_SEP_RECEIVE_ARGS,mpi_sep_receive_args)
FCALLSCFUN3(INT,mpi_sep_send_args,MPI_SEP_SEND_ARGS,mpi_sep_send_args,INT,INT,INT)
FCALLSCFUN6(INT,mpi_sep_tag_bcast,MPI_SEP_SEND_TAG_BCAST,mpi_sep_tag_bcast,INT,STRING,INT,STRING,INT,INT)

FCALLSCFUN7(INT,mpi_sep_tag_bcast_index,MPI_SEP_SEND_TAG_BCAST_INDEX,mpi_sep_tag_bcast_index,INT,STRING,INT,STRING,INT,INT,INT)

FCALLSCFUN8(INT,mpi_sep_tag_sum_index,MPI_SEP_SEND_TAG_SUM_INDEX,mpi_sep_tag_sum_index,INT,STRING,INT,STRING,INT,INT,INT,INT)
FCALLSCFUN7(INT,mpi_sep_tag_sum,MPI_SEP_SEND_TAG_SUM,mpi_sep_tag_sum,INT,STRING,INT,STRING,INT,INT,INT)
FCALLSCFUN7(INT,mpi_sep_tag_distribute,MPI_SEP_SEND_TAG_DISTRIBUTE,mpi_sep_tag_distribute,INT,STRING,INT,STRING,INT,INT,PINT)

FCALLSCFUN9(INT,mpi_sep_tag_combine_big,MPI_SEP_SEND_TAG_COMBINE_BIG,mpi_sep_tag_combine_big,INT,STRING,INT,STRING,INT,INT,INT,PINT,INT)
FCALLSCFUN8(INT,mpi_sep_tag_combine,MPI_SEP_SEND_TAG_COMBINE,mpi_sep_tag_combine,INT,STRING,INT,STRING,INT,INT,PINT,INT)

FCALLSCFUN9(INT,mpi_sep_tag_combine_index,MPI_SEP_SEND_TAG_COMBINE_INDEX,mpi_sep_tag_combine_index,INT,STRING,INT,STRING,INT,INT,PINT,INT,INT)


FCALLSCFUN2(INT,mpi_sep_set_dead,MPI_SEP_SET_DEAD,mpi_sep_set_dead,INT,PINT)
FCALLSCFUN0(INT,mpi_sep_clean,MPI_SEP_CLEAN,mpi_sep_clean)
FCALLSCFUN1(INT,mpi_sep_check_lock,MPI_SEP_CHECK_LOCK,mpi_sep_check_lock,STRING)
FCALLSCFUN1(INT,mpi_sep_open_lock,MPI_SEP_OPEN_LOCK,mpi_sep_open_lock,STRING)
FCALLSCFUN2(INT,mpi_sep_close_lock,MPI_SEP_CLOSE_LOCK,mpi_sep_close_lock,STRING,INT)
FCALLSCFUN2(INT,mpi_sep_create_group,MPI_SEP_CREATE_GROUP,mpi_sep_create_group,INT,PINT)


/*
FCALLSCFUN0(INT,sep_mpi_auto_io_init,SEP_MPI_AUTO_IO_INIT,sep_mpi_auto_io_init)
FCALLSCFUN0(INT,sep_mpi_init,SEP_MPI_INIT,sep_mpi_init)
FCALLSCFUN2(INT,sep_mpi_in_out_tags,SEP_MPI_IN_OUT_TAGS,sep_mpi_in_out_tags,STRING,STRING)
FCALLSCFUN0(INT,sep_mpi_finish,SEP_MPI_FINISH,sep_mpi_finish)
FCALLSCFUN0(INT,sep_mpi_proc_type,SEP_MPI_PROC_TYPE,sep_mpi_proc_type)
FCALLSCFUN2(INT,sep_mpi_thread_portion,SEP_MPI_THREAD_PORTION,sep_mpi_thread_portion,STRING,INT)
FCALLSCFUN5(INT,sep_mpi_distrib_in_tag,SEP_MPI_DISTRIB_IN_TAG,sep_mpi_distrib_in_tag,STRING,STRING,INT,INT,STRING)
FCALLSCFUN2(INT,sep_mpi_proc_begin,SEP_MPI_PROC_BEGIN,sep_mpi_proc_begin,PSTRING,PSTRING)
FCALLSCFUN2(INT,sep_mpi_io_tags,SEP_MPI_IO_TAGS,sep_mpi_io_tags,STRING,STRING)
FCALLSCFUN2(INT,sep_mpi_begin_io,SEP_MPI_BEGIN_IO,sep_mpi_begin_io,STRING,INT)
FCALLSCFUN5(INT,sep_mpi_distrib_out_tag,SEP_MPI_DISTRIB_OUT_TAG,sep_mpi_distrib_out_tag,STRING,STRING,INT,INT,STRING)
FCALLSCFUN3(INT,sep_mpi_update_block,SEP_MPI_UPDATE_BLOCK,sep_mpi_update_block,STRING,INT,INT)
FCALLSCFUN1(INT,sep_mpi_collect_all,SEP_MPI_COLLECT_ALL,sep_mpi_collect_all,STRING)
FCALLSCFUN1(INT,sep_mpi_distrib_all,SEP_MPI_DISTRIB_ALL,sep_mpi_distrib_all,STRING)
*/
#endif
