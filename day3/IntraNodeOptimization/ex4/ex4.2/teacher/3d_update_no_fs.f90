program matrix_upd
use omp_lib

    implicit none
    integer :: i, j, k
    integer :: m, n,  AllocateStatus 
    integer :: id, place_id, nthreads
    real*8, dimension(:,:,:), allocatable :: mat
    real*8 t1, t2, t3
    real*8 :: s

    m = 1500
    n = 1500

    allocate(mat(1:m, 1:n, 1:n), STAT = AllocateStatus)
    if (AllocateStatus /= 0) STOP "*** Not enough memory ***"

    !print some thread affinity attributes. 
    !Let us open a fake parallel section to get the info
    call c_affinity()
    !$omp parallel default(none) private(id, place_id, nthreads)
        nthreads = omp_get_num_threads()
        !$omp single
            print*, "number of threads: ", nthreads;
        !$omp end single nowait
        id = omp_get_thread_num()
        place_id = omp_get_place_num()
        print*, "id: ", id, " in place:", place_id
    !$omp end parallel


    print*, "** Matrix init time start"
    t1 = omp_get_wtime()
        
    call initialize_matrix(mat, m, n)
    
    t2 = omp_get_wtime()
    print*, "** Matrix init elapsed time: ", t2-t1

    s = 0.5d0
    !$omp parallel default(none) private(i,j,k) shared(mat,m,n,s)
    do k = 2, n
        do j = 2, n
            !$omp do schedule(static)
            do i = 1, m
                mat(i,j,k) = mat(i,j,k-1) + mat(i,j-1,k)*s
            enddo
            !$omp end do nowait
        enddo
    enddo
    !$omp end parallel
    
    t3 = omp_get_wtime()
    print*, "** Matrix update elapsed time: ", t3-t2
end program

subroutine initialize_matrix(x, m, n)
    implicit none
    integer :: i, j, k
    integer :: m, n
    real*8 :: x(1:m, 1:n, 1:n)

    !initialization
    do k = 1, n
        do j = 1, n
            do i = 1, m
                x(i,j,k) = real(i+j*k) 
            enddo
        enddo
    enddo
end subroutine

