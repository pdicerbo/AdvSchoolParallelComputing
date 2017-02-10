!
! Jacobi iteration with Blocking Sends and Receives 
! Modified from Archer Training Material (EPCC)
!
! http://www.archer.ac.uk/training/course-material/2016/09/160929_AdvMPI_EPCC/index.php
! 
program jacobi
  use mpi
  implicit none     
  ! Boundary value at the LHS of the pipe
  real(kind=8), parameter :: LEFT = 1.0 
  ! Boundary value at the RHS of the pipe
  real(kind=8), parameter :: RIGHT = 10.0
  ! The convergence to terminate at
  real(kind=8), parameter :: CONVERGENCE_ACCURACY = 1e-4
  ! The maximum number of iterations
  integer, parameter :: MAX_ITERATIONS = 500000
  ! How often to report the norm
  integer, parameter :: REPORT_NORM_PERIOD = 1000

  real(kind=8), dimension(:,:), allocatable :: grid, grid_new, temp
  real(kind=8) :: bnorm, rnorm, norm, tmpnorm
  integer :: i, j, k, ierr, size, myrank, local_nx, nx, ny, requests(4)
  character(len=32) :: arg
  double precision :: start_time

  call mpi_init(ierr)
  call mpi_comm_size(MPI_COMM_WORLD, size, ierr)
  call mpi_comm_rank(MPI_COMM_WORLD, myrank, ierr)
  start_time=MPI_Wtime()

  if (myrank ==0 .and. command_argument_count() /= 2) then
     print *, &
          "You must provide two command line arguments, the global size in X and the global size in Y"
     stop
  end if

  call get_command_argument(1, arg)
  read(arg,*) nx
  call get_command_argument(2, arg)
  read(arg,*) ny

  if (myrank==0) then
     print *, "Global size in X=", nx, "Global size in Y=", ny
  end if

  local_nx=nx/size
  if (local_nx * size .lt. nx) then
     if (myrank .lt. nx - local_nx * size) local_nx=local_nx+1
  end if

  allocate(grid(ny, 0:local_nx+1), grid_new(ny, 0:local_nx+1), temp(ny, 0:local_nx+1))

  bnorm=0.0
  tmpnorm=0.0
  rnorm=0.0

  ! intialise values
  if (myrank ==0) then
     grid(:,0)=LEFT
  else
     grid(:,0)=0.0d0
  endif
  if (myrank ==size-1) then
     grid(:,local_nx+1)=RIGHT
  else
     grid(:,local_nx+1)=0.d0
  endif
  grid(:,1:local_nx)=0.d0
  grid_new=grid


  do j=1, local_nx
     do i=1, ny
        tmpnorm=tmpnorm+((grid(i,j)*4-grid(i-1,j)-grid(i+1,j)-grid(i,j-1)-grid(i,j+1)) ** 2)
     end do
  end do
  call mpi_allreduce(tmpnorm, bnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD, ierr)
  bnorm=sqrt(bnorm)


  do k=0, MAX_ITERATIONS
     if (myrank .gt. 0) then
        call mpi_send(grid(1,1), ny, MPI_DOUBLE, myrank-1, 0, MPI_COMM_WORLD, requests(1), ierr)
        call mpi_recv(grid(1,0), ny, MPI_DOUBLE, myrank-1, 0, MPI_COMM_WORLD, requests(2), ierr)
     end if
     if (myrank .lt. size-1) then
        call mpi_send(grid(1,local_nx), ny, MPI_DOUBLE, myrank+1, 0, MPI_COMM_WORLD, requests(3), ierr)
        call mpi_recv(grid(1,local_nx+1), ny, MPI_DOUBLE, myrank+1, 0, MPI_COMM_WORLD, requests(4), ierr)
     end if
     tmpnorm=0.0
     do j=1, local_nx
        do i=1, ny
           tmpnorm=tmpnorm+((grid(i,j)*4-grid(i-1,j)-grid(i+1,j)-grid(i,j-1)-grid(i,j+1)) ** 2)
        end do
     end do
     call mpi_allreduce(tmpnorm, rnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD, ierr)
     norm=sqrt(rnorm)/bnorm
     if (norm .lt. CONVERGENCE_ACCURACY) exit

     do j=1, local_nx
        do i=1, ny
           grid_new(i,j)=0.25 * (grid(i-1,j) + grid(i+1,j) + grid(i,j-1) + grid(i,j+1))
        end do
     end do
     temp=grid_new
     grid_new=grid
     grid=temp

     if (mod(k, REPORT_NORM_PERIOD)==0 .and. myrank==0) print *, "Iteration=",k," Relative Norm=",norm
  end do
  if (myrank==0) print *, "Terminated on ",k," iterations, Relative Norm=", norm, " runtime=",MPI_Wtime()-start_time," sec"
  deallocate(grid, grid_new, temp)
  call mpi_finalize(ierr)
end program jacobi

