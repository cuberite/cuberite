pipeline {
    options {
      timeout(time: 1, unit: 'HOURS') 
    }
    agent {
        docker 'cuberite/docker-ci/minimal:latest'
    }
    stages {
        stage("Prepare") {
            steps {
                sh 'git submodule update --init'
            }
        }
        /* stage("Check") {
            parallel {
                stage("CheckBasicStyle") {
                    steps {
                        dir("src") {
                            sh 'find . -name \\*.cpp -or -name \\*.h > AllFiles.lst'
                            sh 'lua CheckBasicStyle.lua'
                            sh 'cd Bindings && lua CheckBindingsDependencies.lua'
                        }
                    }
                }
                stage("clang-tidy") {
                    steps {
                        sh './clang-tidy.sh -j 4'
                    }
                }
            }
        } */
        stage("Build") {
		    environment {
                CI_CUBERITE_BUILD_TYPE = 'Debug'
                CI_JOB_NUMBER = "{$env.BUILD_ID}"
                CC = "clang"
                CXX = "clang++"
            }
            steps {
                sh 'bash ./cibuild.sh'
            }

            /* parallel {
                stage("gcc") {
                    environment {
                        CI_CUBERITE_BUILD_TYPE = 'Release'
                        CI_JOB_NUMBER = "{$env.BUILD_ID}"
                        CC = "gcc"
                        CXX = "g++"
                    }
                    steps {
                        sh 'bash ./cibuild.sh'
                    }
                }
                stage("clang") {
                    environment {
                        CI_CUBERITE_BUILD_TYPE = 'Debug'
                        CI_JOB_NUMBER = "{$env.BUILD_ID}"
                        CC = "clang"
                        CXX = "clang++"
                    }
                    steps {
                        sh 'bash ./cibuild.sh'
                    }
                }
            } */
        }
		stage("Artifacts") {
			when {
				branch 'master'
			}
            steps {
                archiveArtifacts artifacts: 'gcc_Release/Server/.luacheckrc'
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
